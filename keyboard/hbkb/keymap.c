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

/*
 * Keymap for Macway mod
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "keymap.h"


#define KEYMAP( \
    K22, K23, K33, K43, K53, K52, K72, K73, K83, K93, KA3, KA2, K82, K64, K61, \
    K21, K24, K34, K44, K54, K51, K71, K74, K84, K94, KA4, KA1, K81, K65, \
    K31, K25, K35, K45, K55, K50, K70, K75, K85, K95, KA5, KA0, K66, \
    K11, K26, K36, K46, K56, K57, K77, K76, K86, K96, KA7, K16, \
    K02, KB0,      K67,      KB7, K06 \
) { \
    { KC_NO,    KC_NO,    KC_##K02, KC_NO,    KC_NO,    KC_NO,    KC_##K06, KC_NO    }, \
    { KC_NO,    KC_##K11, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##K16, KC_NO    }, \
    { KC_NO,    KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_NO    }, \
    { KC_NO,    KC_##K31, KC_NO,    KC_##K33, KC_##K34, KC_##K35, KC_##K36, KC_NO    }, \
    { KC_NO,    KC_NO,    KC_NO,    KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_NO    }, \
    { KC_##K50, KC_##K51, KC_##K52, KC_##K53, KC_##K54, KC_##K55, KC_##K56, KC_##K57 }, \
    { KC_NO,    KC_##K61, KC_NO,    KC_NO,    KC_##K64, KC_##K65, KC_##K66, KC_##K67 }, \
    { KC_##K70, KC_##K71, KC_##K72, KC_##K73, KC_##K74, KC_##K75, KC_##K76, KC_##K77 }, \
    { KC_NO,    KC_##K81, KC_##K82, KC_##K83, KC_##K84, KC_##K85, KC_##K86, KC_NO    }, \
    { KC_NO,    KC_NO,    KC_NO,    KC_##K93, KC_##K94, KC_##K95, KC_##K96, KC_NO    }, \
    { KC_##KA0, KC_##KA1, KC_##KA2, KC_##KA3, KC_##KA4, KC_##KA5, KC_NO,    KC_##KA7 }, \
    { KC_##KB0, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##KB7 }, \
}

#define KEYCODE(layer, row, col) (pgm_read_byte(&keymaps[(layer)][(row)][(col)]))


// Assign Fn key(0-7) to a layer to which switch with the Fn key pressed.
static const uint8_t PROGMEM fn_layer[] = {
    0,              // Fn0
    0,              // Fn1
    0,              // Fn2
    0,              // Fn3
    0,              // Fn4
    0,              // Fn5
    0,              // Fn6
    0               // Fn7
};

// Assign Fn key(0-7) to a keycode sent when release Fn key without use of the layer.
// See layer.c for details.
static const uint8_t PROGMEM fn_keycode[] = {
    KC_NO,          // Fn0
    KC_NO,          // Fn1
    KC_SLSH,        // Fn2
    KC_SCLN,        // Fn3
    KC_SPC,         // Fn4
    KC_NO,          // Fn5
    KC_NO,          // Fn6
    KC_NO           // Fn7
};

/*
 * ,---.                              ,-----------. ,--------------.
 * |Esc| mut|vd | vu|thinkvantage     |prt|scr|pau| |ins|home|pgpu |
 * |-------------. ,-------------.  ,--------------| |-------------|
 * | f1| f2|f3|f4| |f5| f6| f7|f8|  |f9|f10|f11|f12| |del|end |pgdn|
 * |---------------------------------------------------------------|
 * |` |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|backspace   |
 * |-----------------------------------------------------------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| \   |
 * |-----------------------------------------------------------|
 * |EscCap|  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|Return  |
 * |-----------------------------------------------------------|
 * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|Shift     |
 * |-----------------------------------------------------------|
 * |Fn|Ctrl|sup|Alt|           Space    |Alt|men|ctr|bac|up|for|
 * `-------------------------------------------------lef|dn|rig|
 *                                                   `---------'
 *
 *
 * Matrix: 16x8
 * 8 col
 * 16 row
 * TRANSPOSE THIS
 *    |       0 |       1 |       2 |       3 |       4 |       5 |       6 |       7 |       8 |       9 |      10 |      10 |      12 |      13 |      14 |      15
 * ---+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------
 *  0 | `       | F1      | F2      | 5       | 6       | =       | F8      | -       | F9      | INSERT  | Delete  | PageUp  | Home    |         |         | L_CTRL
 *  1 | 1       | 2       | 3       | 4       | 7       | 8       | 9       | 0       | F10     | F12     | F11     | PgDn    | End     | PrntScr |         |
 *  2 | q       | w       | e       | r       | u       | i       | o       | p       |         |         | VolUp   | L_GUI   |         | ScrLck  |         |
 *  3 | Tab     | CapsLck | F3      | t       | y       | ]       | F7      | [       | Backspac|         | VolDown |         |         |         | L_SHIFT |
 *  4 | a       | s       | d       | f       | j       | k       | l       | ;       | \       |         | Mute    | R_GUI   |         |         |         |
 *  5 | Esc     |         | F4      | g       | h       | F6      |         | '       | F5      |         | thinkVan|         | Up      | L_ALT   |         |
 *  6 | z       | x       | c       | v       | m       | ,       | .       |         | Return  |         |         | Prev    | Pause   |         | R_SHIFT | R_CTRL
 *  7 |         |         |         | b       | n       |         |         | /       | ' '     | R_ARROW | D_ARROW | Next    | L_ARROW | R_ALT   |         |
 *  Transposition thanks to Max: https://pastebin.com/raw/525Q7N7B
 *
 *  Original matrix here: https://github.com/rampadc/arduino-thinkpadkb-usb/blob/master/MicroThinkPadAdapter/MicroThinkPadAdapter.ino#L78
 *  And here: https://flashandrc.files.wordpress.com/2014/08/fil3zp5hw4ojq7d-large.jpg
 *
 * TRANSPOSE THIS
 *    |       0 |       1 |       2 |       3 |       4 |       5 |       6 |       7 |       8 |       9 |      10 |      10 |      12 |      13 |      14 |      15
 * ---+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------
 *  0 | `       | F1      | F2      | 5       | 6       | =       | F8      | -       | F9      | INSERT  | Delete  | PageUp  | Home    |         |         | CapsLock
 *  1 | 1       | 2       | 3       | 4       | 7       | 8       | 9       | 0       | F10     | F12     | F11     | PgDn    | End     | PrntScr |         |
 *  2 | q       | w       | e       | r       | u       | i       | o       | p       |         |         | VolUp   | L_GUI   |         | ScrLck  |         |
 *  3 | Tab     | L_CTRL  | F3      | t       | y       | ]       | F7      | [       | \       |         | VolDown |         |         |         | L_SHIFT |
 *  4 | a       | s       | d       | f       | j       | k       | l       | ;       | BackSpac|         | Mute    | R_GUI   |         |         |         |
 *  5 | Esc     |         | F4      | g       | h       | F6      |         | '       | F5      |         | thinkVan|         | Up      | L_ALT   |         |
 *  6 | z       | x       | c       | v       | m       | ,       | .       |         | Return  |         |         | Prev    | Pause   |         | R_SHIFT | R_CTRL
 *  7 |         |         |         | b       | n       |         |         | /       | ' '     | R_ARROW | D_ARROW | Next    | L_ARROW | R_ALT   |         |
 *  fn is some funky shit
 *  alt is not in the keymap?
 *  super and menu are alt.
 *  Pinout of connector: https://cdn.instructables.com/ORIG/FHW/IFU1/HUFW65MQ/FHWIFU1HUFW65MQ.png
 *
 */
static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    {
     //   00            01          02          03          04          05          06          07
/* 00 */ {KC_GRV,	    KC_1,	    KC_Q,	    KC_TAB,	    KC_A,	    KC_ESC,	    KC_Z,	    KC_NO},
/* 01 */ {KC_F1,	    KC_2,	    KC_W,	    KC_LCTL,    KC_S,	    KC_NO,	    KC_X,	    KC_NO},
/* 02 */ {KC_F2,	    KC_3,	    KC_E,	    KC_F3,	    KC_D,	    KC_F4,	    KC_C,	    KC_NO},
/* 03 */ {KC_5,	        KC_4,	    KC_R,	    KC_T,	    KC_F,	    KC_G,	    KC_V,	    KC_B},
/* 04 */ {KC_6,	        KC_7,	    KC_U,	    KC_Y,	    KC_J,	    KC_H,	    KC_M,	    KC_N},
/* 05 */ {KC_EQL,	    KC_8,	    KC_I,	    KC_RBRC,    KC_K,	    KC_F6,	    KC_COMM,	KC_NO},
/* 06 */ {KC_F8,	    KC_9,	    KC_O,	    KC_F7,	    KC_L,	    KC_NO,	    KC_DOT,	    KC_NO},
/* 07 */ {KC_MINS,	    KC_0,	    KC_P,	    KC_LBRC,    KC_SCLN,    KC_QUOT,    KC_NO,	    KC_PSLS},
/* 08 */ {KC_F9,	    KC_F10,	    KC_NO,	    KC_BSLS,    KC_BSPC,    KC_F5,	    KC_ENT,	    KC_SPC},
/* 09 */ {KC_INS,	    KC_F12,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_RIGHT},
/* 10 */ {KC_DEL,	    KC_F11,	    KC_VOLU,    KC_VOLD,    KC_MUTE,    KC_NO,	    KC_NO,	    KC_DOWN},
/* 11 */ {KC_PGUP,	    KC_PGDN,    KC_LGUI,    KC_NO,	    KC_RGUI,    KC_NO,	    KC_WBAK,	KC_WFWD},
/* 12 */ {KC_HOME,	    KC_END,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_UP,	    KC_PAUSE,	KC_LEFT},
/* 13 */ {KC_NO,	    KC_PSCR,    KC_SLCK,    KC_NO,	    KC_NO,	    KC_LALT,    KC_NO,	    KC_RALT},
/* 14 */ {KC_NO,	    KC_NO,	    KC_NO,	    KC_LSFT,    KC_NO,	    KC_NO,	    KC_RSFT,	KC_NO},
/* 15 */ {KC_CAPS,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_NO,	    KC_RCTL,	KC_N}
     }
//    /* Layer 0: Default Layer
//     * ,-----------------------------------------------------------.
//     * |Esc|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|  \|  `|
//     * |-----------------------------------------------------------|
//     * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]| BS  |
//     * |-----------------------------------------------------------|
//     * |Contro|  A|  S|  D|  F|  G|  H|  J|  K|  L|Fn3|  '|Return  |
//     * |-----------------------------------------------------------|
//     * |Shift   |  Z|  X|  C|  V|  B|  N|  M|  ,|  .|Fn2|Shift     |
//     * |-----------------------------------------------------------|
//     * |Ctrl |   |Alt  |           Fn4             |Alt  |   |Fn1  |
//     * `-----'   `---------------------------------------'   `-----'
//     */
//    KEYMAP(ESC, 1,   2,   3,   4,   5,   6,   7,   8,   9,   0,   MINS,EQL, BSLS,GRV, \
//           TAB, Q,   W,   E,   R,   T,   Y,   U,   I,   O,   P,   LBRC,RBRC,BSPC, \
//           LCTL,A,   S,   D,   F,   G,   H,   J,   K,   L,   FN3, QUOT,ENT, \
//           LSFT,Z,   X,   C,   V,   B,   N,   M,   COMM,DOT, FN2, FN1, \
//           LGUI,     LALT,          FN4,           RALT,     FN1),
//
//
//    /* Layer 1: HHKB mode (HHKB Fn)
//     * ,-----------------------------------------------------------.
//     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Ins|Del|
//     * |-----------------------------------------------------------|
//     * |Caps |   |   |   |   |   |   |   |Psc|Slk|Pus|Up |   |     |
//     * |-----------------------------------------------------------|
//     * |Contro|VoD|VoU|Mut|   |   |  *|  /|Hom|PgU|Lef|Rig|        |
//     * |-----------------------------------------------------------|
//     * |Shift   |   |   |   |   |   |  +|  -|End|PgD|Dow|Shift     |
//     * |-----------------------------------------------------------|
//     * |Ctrl |   |Alt  |           Space           |Alt  |   |Fn1  |
//     * `-----'   `---------------------------------------'   `-----'
//     */
//    KEYMAP(ESC, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, INS, DEL, \
//           CAPS,NO,  NO,  NO,  NO,  NO,  NO,  NO,  PSCR,SLCK,BRK, UP,  NO,  NO, \
//           LCTL,VOLD,VOLU,MUTE,NO,  NO,  PAST,PSLS,HOME,PGUP,LEFT,RGHT,NO, \
//           LSFT,NO,  NO,  NO,  NO,  NO,  PPLS,PMNS,END, PGDN,DOWN,FN1, \
//           LGUI,     LALT,          SPC,           RALT,     FN1),
//
//
//    /* Layer 2: Vi mode (Quote/Rmeta)
//     * ,-----------------------------------------------------------.
//     * |  `| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|   |   |
//     * |-----------------------------------------------------------|
//     * |  \  |Hom|PgD|Up |PgU|End|Hom|PgD|PgU|End|   |   |   |     |
//     * |-----------------------------------------------------------|
//     * |Contro|   |Lef|Dow|Rig|   |Lef|Dow|Up |Rig|   |   |        |
//     * |-----------------------------------------------------------|
//     * |Shift   |   |   |   |   |   |Hom|PgD|PgU|End|xxx|Shift     |
//     * |-----------------------------------------------------------|
//     * |Ctrl |   |Alt  |           Space           |Alt  |   |Fn1  |
//     * `-----'   `---------------------------------------'   `-----'
//     */
//    KEYMAP(GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, NO,  NO, \
//           BSLS,HOME,PGDN,UP,  PGUP,END, HOME,PGDN,PGUP,END, NO,  NO,  NO,  NO, \
//           LCTL,NO,  LEFT,DOWN,RGHT,NO,  LEFT,DOWN,UP,  RGHT,NO,  NO,  NO, \
//           LSFT,NO,  NO,  NO,  NO,  NO,  HOME,PGDN,PGUP,END, FN2, RSFT, \
//           LGUI,     LALT,          SPC,           RALT,     NO),
//
//
//
//    /* Layer 3: Mouse mode (Semicolon)
//     * ,-----------------------------------------------------------.
//     * |  `| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|   |   |
//     * |-----------------------------------------------------------|
//     * |  \  |MwL|MwD|McU|MwU|MwR|MwL|MwD|MwU|MwR|   |   |   |     |
//     * |-----------------------------------------------------------|
//     * |Contro|   |McL|McD|McR|   |McL|McD|McU|McR|xxx|   |        |
//     * |-----------------------------------------------------------|
//     * |Shift   |   |   |Mb1|Mb2|Mb3|Mb2|Mb1|   |   |   |Shift     |
//     * |-----------------------------------------------------------|
//     * |Ctrl |   |Alt  |           Space           |Alt  |   |     |
//     * `-----'   `---------------------------------------'   `-----'
//     * Mc: Mouse Cursor / Mb: Mouse Button / Mw: Mouse Wheel
//     */
//    KEYMAP(GRV, F1,  F2,  F3,  F4,  F5,  F6,  F7,  F8,  F9,  F10, F11, F12, NO, NO, \
//           BSLS,WH_L,WH_D,MS_U,WH_U,WH_R,WH_L,WH_D,WH_U,WH_R,NO,  NO,  NO,  NO, \
//           LCTL,NO,  MS_L,MS_D,MS_R,NO,  MS_L,MS_D,MS_U,MS_R,FN3, NO,  NO, \
//           LSFT,NO,  NO,  BTN1,BTN2,BTN3,BTN2,BTN1,NO,  NO,  NO,  RSFT, \
//           LGUI,     LALT,          BTN1,          RALT,     NO),
//
//
//    /* Layer 4: Matias half keyboard style (Space)
//     * ,-----------------------------------------------------------.
//     * |Esc| F1| F2| F3| F4| F5| F6| F7| F8| F9|F10|F11|F12|Delete |
//     * |-----------------------------------------------------------|
//     * |Backs|  P|  O|  I|  U|  Y|  T|  R|  E|  W|  Q|   |   |Tab  |
//     * |-----------------------------------------------------------|
//     * |Contro|  ;|  L|  K|  J|  H|  G|  F|  D|  S|  A|Con|Control |
//     * |-----------------------------------------------------------|
//     * |Shift   |  /|  .|  ,|  M|  N|  B|  V|  C|  X|  Z|Shift     |
//     * |-----------------------------------------------------------|
//     * |Ctrl |   |Alt  |           Fn4             |Alt  |   |     |
//     * `-----'   `---------------------------------------'   `-----'
//     */
//    KEYMAP(MINS,0,   9,   8,   7,   6,   5,   4,   3,   2,   1,   NO,  NO,  NO, ESC, \
//           BSPC,P,   O,   I,   U,   Y,   T,   R,   E,   W,   Q,   NO,  NO,  TAB, \
//           LCTL,SCLN,L,   K,   J,   H,   G,   F,   D,   S,   A,   RCTL,RCTL, \
//           LSFT,SLSH,DOT, COMM,M,   N,   B,   V,   C,   X,   Z,   RSFT, \
//           LGUI,     LALT,          FN4,           RALT,     NO),

};


uint8_t keymap_get_keycode(uint8_t layer, uint8_t row, uint8_t col)
{
    return KEYCODE(layer, row, col);
}

uint8_t keymap_fn_layer(uint8_t index)
{
    return pgm_read_byte(&fn_layer[index]);
}

uint8_t keymap_fn_keycode(uint8_t index)
{
    return pgm_read_byte(&fn_keycode[index]);
}
