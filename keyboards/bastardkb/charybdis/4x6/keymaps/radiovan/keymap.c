/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_SYM,
    LAYER_NUM,
    LAYER_NAV,
    LAYER_POINTER,
    LAYER_POINTER_QUICK,
};

enum custom_keycodes {
    DBLPAR = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case DBLPAR:
        if (record->event.pressed) {
            SEND_STRING("()"SS_TAP(X_LEFT));
        } else {
        }
        break;
    }
    return true;
};

enum {
    TD_COL,
    TD_MIN,
    TD_PIP,
    TD_TIL,
    TD_UND,

    TD_PAR,
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_COL] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_COLN),
    [TD_MIN] = ACTION_TAP_DANCE_DOUBLE(KC_I, KC_MINS),
    [TD_PIP] = ACTION_TAP_DANCE_DOUBLE(KC_M, KC_PIPE),
    [TD_TIL] = ACTION_TAP_DANCE_DOUBLE(KC_V, KC_TILD),
    [TD_UND] = ACTION_TAP_DANCE_DOUBLE(KC_U, KC_UNDS),

    [TD_PAR] = ACTION_TAP_DANCE_DOUBLE(KC_RPRN, DBLPAR),
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
// #define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

/*
#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 800
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 1
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
*/

#define SYM LT(LAYER_SYM, KC_SPC)
#define NUM LT(LAYER_NUM, KC_BSPC)
#define NAV LT(LAYER_NAV, KC_ENT)
#define PT_ESC LT(LAYER_POINTER, KC_ESC)
#define PT_INS LT(LAYER_POINTER, KC_INS)
#define PT_SL LT(LAYER_POINTER, KC_SLSH)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_L LT(LAYER_POINTER, KC_L)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

const uint16_t PROGMEM combo_esc[] = {KC_F, KC_V, COMBO_END};
const uint16_t PROGMEM combo_ent[] = {KC_J, KC_M, COMBO_END};
const uint16_t PROGMEM combo_tab[] = {LCTL_T(KC_D), KC_F, COMBO_END};
const uint16_t PROGMEM combo_bks[] = {KC_J, KC_K, COMBO_END};
const uint16_t PROGMEM combo_bks_num[] = {KC_4, KC_5, COMBO_END};
const uint16_t PROGMEM combo_bks_sym[] = {KC_CIRC, KC_PERC, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_esc, KC_ESC),
    COMBO(combo_ent, KC_ENT),
    COMBO(combo_tab, KC_TAB),
    COMBO(combo_bks, KC_BSPC),
    COMBO(combo_bks_num, KC_BSPC),
    COMBO(combo_bks_sym, KC_BSPC),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                KC_GRV,        KC_1,        KC_2,        KC_3,        KC_4,        KC_5,           KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_RBRC,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_QUOT,       KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,           KC_Y,    TD(TD_UND),   TD(TD_MIN),     KC_O,        KC_P,        KC_LBRC,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_BSLS,       KC_A,        KC_S, LCTL_T(KC_D),       KC_F,    LGUI_T(KC_G),       KC_H,        KC_J,        KC_K,        KC_L,     TD(TD_COL),     KC_QUOT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                PT_ESC,        PT_Z,        KC_X,        KC_C,    TD(TD_TIL),      KC_B,           KC_N,     TD(TD_PIP),     KC_COMM,     KC_DOT,      PT_SL,       PT_INS,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                      LCTL(KC_B),     KC_LALT, OSM(MOD_LGUI),      SYM,       NUM,
                                                                OSM(MOD_LCTL), OSM(MOD_LSFT),      NAV
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_SYM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                XXXXXXX,       KC_1,        KC_2,        KC_3,        KC_4,        KC_5,           KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       KC_PLUS,     KC_LBRC,     KC_RBRC,        KC_EXLM,     KC_AT,       KC_HASH,     KC_ASTR,     KC_UNDS,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       KC_EQL,      KC_LPRN,  TD(TD_PAR),        KC_AMPR,     KC_CIRC,     KC_PERC,     KC_DLR,      KC_SCLN,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                _______,       KC_BSLS,   XXXXXXX,       KC_MINS,     KC_LCBR,     KC_RCBR,        KC_LT,       KC_GT,       KC_PIPE,     _______,     _______,     _______,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         KC_GRV,      KC_DQT,      KC_QUOT,        _______,     XXXXXXX,
                                                                      KC_UNDS,     KC_TILD,        XXXXXXX
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                XXXXXXX,       KC_F1,     KC_F2,         KC_F3,       KC_F4,       KC_F5,          XXXXXXX,     KC_7,       KC_8,     KC_9,      KC_0,        KC_MINS,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_F6,     KC_F7,         KC_F8,       KC_F9,       KC_F10,         XXXXXXX,     KC_7,       KC_8,     KC_9,      KC_UNDS,     KC_DQT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     DBLPAR,         KC_DOT,      KC_4,       KC_5,     KC_6,      _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        KC_0,        KC_1,       KC_2,     KC_3,      _______,     KC_BSLS,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      CW_TOGG,     KC_SPC,         XXXXXXX,     _______,
                                                                      KC_DOT,      KC_0,           XXXXXXX
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                RM_TOGG,       XXXXXXX,   UG_SPDD,       UG_SPDU,     KC_VOLD,     KC_VOLU,        XXXXXXX,     XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_NEXT,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_VALD,     UG_VALU,        KC_HOME,     KC_END,     XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_PREV,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_HUED,     UG_HUEU,        KC_LEFT,     KC_DOWN,    KC_UP,    KC_RGHT,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_TOGG,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_SATD,     UG_SATU,        KC_PGDN,     KC_PGUP,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         KC_HOME,     KC_TAB,      KC_END,         XXXXXXX,     XXXXXXX,
                                                                      KC_PGUP,     KC_PGDN,        _______  
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                QK_BOOT,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        XXXXXXX,     XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     QK_BOOT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        XXXXXXX,     XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        KC_BTN2,     KC_BTN1,    KC_BTN3,  DRG_TOG,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        XXXXXXX,     XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         DRGSCRL,     KC_BTN3,     SNIPING,        DRG_TOG,     XXXXXXX,
                                                                      KC_BTN2,     KC_BTN1,        SNP_TOG
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_POINTER_QUICK] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                _______,       _______,   _______,       _______,     _______,     _______,        _______,     _______,    _______,  _______,   _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                _______,       _______,   _______,       _______,     _______,     _______,        _______,     _______,    _______,  _______,   _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                _______,       _______,   _______,       _______,     _______,     _______,        KC_BTN2,     KC_BTN1,    KC_BTN3,  _______,   _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                _______,       _______,   _______,       _______,     _______,     _______,        _______,     _______,    _______,  _______,   _______,     _______,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         DRGSCRL,     SNIPING,     KC_BTN3,        SNP_TOG,     DRG_TOG,
                                                                      KC_BTN2,     KC_BTN1,        _______
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),
};
// clang-format on

/*
#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (!layer_state_is(LAYER_POINTER)) {
        if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
            if (auto_pointer_layer_timer == 0) {
                layer_on(LAYER_POINTER_QUICK);
#        ifdef RGB_MATRIX_ENABLE
                rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
                rgb_matrix_sethsv_noeeprom(HSV_RED);
#        endif // RGB_MATRIX_ENABLE
            }
            auto_pointer_layer_timer = timer_read();
        }
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER_QUICK);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

//#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
//layer_state_t layer_state_set_user(layer_state_t state) {
//    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
//    return state;
//}
//#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE
*/

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif
