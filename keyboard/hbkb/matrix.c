/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


/*
 * T60 stuff
 *
 * Pin usage:
 *   COL: PB0-7
 *   ROW:
 *      D 0,1,2,3,4; 6,7
 *      F 0,1; 4,5,6,7
 *      C 6,7
 *      E 6
 *
 *   COL: 8
 *   ROW: 16
 *   https://cdn.instructables.com/ORIG/FG4/BSFY/HUFW65ZZ/FG4BSFYHUFW65ZZ.png
 */
#ifndef DEBOUNCE
#   define DEBOUNCE	10
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);


void matrix_init(void)
{
    // JTAG disable for PORT F. write JTD bit twice within four cycles.
    MCUCR |= (1<<JTD);
    MCUCR |= (1<<JTD);

    // initialize rows
    unselect_rows();

    // initialize columns to input with pull-up(DDR:0, PORT:1)
    DDRB = 0x00;
    PORTB = 0xFF;

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

inline
static matrix_row_t read_cols(void)
{
    return ~PINB;
}

inline
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    // D 0,1,2,3,4; 6,7
    // F 0,1; 4,5,6,7
    // C 6,7
    // E 6
    DDRD  &= ~0b11011111;
    PORTD &= ~0b11011111;
    DDRF  &= ~0b11110011;
    PORTF &= ~0b11110011;
    DDRC  &= ~0b11000000;
    PORTC &= ~0b11000000;
    DDRE  &= ~0b01000000;
    PORTE &= ~0b01000000;
}

inline
static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    // D 0,1,2,3,4; 6,7
    // F 0,1; 4,5,6,7
    // C 6,7
    // E 6
    //
    // 0-4: D0-D4
    // 5-6: D6-7
    // 7-8: F0-1
    // 9-12: F4-7
    // 13-14: C6-7
    // 15: E6
    switch (row) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            DDRD  |=  (1<<row);
            PORTD &= ~(1<<row);
            break;
        case 5:
        case 6:
            DDRD  |=  (1<<row+1);
            PORTD &= ~(1<<row+1);
            break;
        case 7:
        case 8:
            DDRF  |=  (1<<row-7);
            PORTF &= ~(1<<row-7);
            break;
        case 9:
        case 10:
        case 11:
        case 12:
            DDRF  |=  (1<<(row-5));
            PORTF &= ~(1<<(row-5));
            break;
        case 13:
        case 14:
            DDRC  |=  (1<<(row-7));
            PORTC &= ~(1<<(row-7));
            break;
        case 15:
            DDRE  |=  (1<<6);
            PORTE &= ~(1<<6);
            break;
    }
}
