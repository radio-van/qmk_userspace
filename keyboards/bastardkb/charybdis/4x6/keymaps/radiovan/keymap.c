// vim: foldmethod=marker:

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
    LAYER_NUM2,
};

// {{{ ----- POINTER -----

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

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

// }}} ----- POINTER -----


#define SYM LT(LAYER_SYM, KC_SPC)
#define NUM TT(LAYER_NUM)
#define NAV LT(LAYER_NAV, KC_ENT)
#define PT_ESC LT(LAYER_POINTER, KC_ESC)
#define PT_INS LT(LAYER_POINTER, KC_INS)


// {{{ ----- TAP DANCE -----

enum {
    TD_COL,
    TD_MIN,
    TD_PIP,
    TD_TIL,
    TD_UND,

    TD_PON,

    TD_PAR,
    TD_BRC,
    TD_CBR,
    TD_DQT,
    TD_QUT,
    TD_GRV,
};

void dance_par(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING(")");
            break;
        case 2:
            SEND_STRING("()"SS_TAP(X_LEFT));
            reset_tap_dance(state);
            break;
    }
}

void dance_brc(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("]");
            break;
        case 2:
            SEND_STRING("[]"SS_TAP(X_LEFT));
            reset_tap_dance(state);
            break;
    }
}

void dance_cbr(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("}");
            break;
        case 2:
            SEND_STRING("{}"SS_TAP(X_LEFT));
            reset_tap_dance(state);
            break;
    }
}

void dance_dqt(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("\"");
            break;
        case 2:
            SEND_STRING("\"\""SS_TAP(X_LEFT));
            reset_tap_dance(state);
            break;
    }
}

void dance_qut(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("'");
            break;
        case 2:
            SEND_STRING("''"SS_TAP(X_LEFT));
            reset_tap_dance(state);
            break;
    }
}

void dance_grv(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("`");
            break;
        case 2:
            SEND_STRING("``"SS_TAP(X_LEFT));
            reset_tap_dance(state);
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_COL] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_COLN),
    [TD_MIN] = ACTION_TAP_DANCE_DOUBLE(KC_I, KC_MINS),
    [TD_PIP] = ACTION_TAP_DANCE_DOUBLE(KC_M, KC_PIPE),
    [TD_TIL] = ACTION_TAP_DANCE_DOUBLE(KC_V, KC_TILD),
    [TD_UND] = ACTION_TAP_DANCE_DOUBLE(KC_U, KC_UNDS),

    [TD_PON] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_COMM, LAYER_POINTER),

    [TD_PAR] = ACTION_TAP_DANCE_FN(dance_par),
    [TD_BRC] = ACTION_TAP_DANCE_FN(dance_brc),
    [TD_CBR] = ACTION_TAP_DANCE_FN(dance_cbr),

    [TD_DQT] = ACTION_TAP_DANCE_FN(dance_dqt),
    [TD_QUT] = ACTION_TAP_DANCE_FN(dance_qut),
    [TD_GRV] = ACTION_TAP_DANCE_FN(dance_grv)
};


// }}} ----- TAP DANCE -----


// {{{ ----- COMBOS -----

const uint16_t PROGMEM combo_ent_3_1[] = {LSFT_T(KC_J), LCTL_T(KC_K), KC_L, COMBO_END};
const uint16_t PROGMEM combo_ent_3_2[] = {LSFT_T(KC_1), LCTL_T(KC_0), KC_PLUS, COMBO_END};

const uint16_t PROGMEM combo_esc_3_1[] = {KC_S, LCTL_T(KC_D), LSFT_T(KC_F), COMBO_END};
const uint16_t PROGMEM combo_esc_3_2[] = {KC_MINS, LCTL_T(KC_6), LSFT_T(KC_5), COMBO_END};

const uint16_t PROGMEM combo_tab_1[] = {LCTL_T(KC_D), LSFT_T(KC_F), COMBO_END};
const uint16_t PROGMEM combo_tab_2[] = {LCTL_T(KC_6), LSFT_T(KC_5), COMBO_END};

const uint16_t PROGMEM combo_bks_1[] = {LSFT_T(KC_J), LCTL_T(KC_K), COMBO_END};
const uint16_t PROGMEM combo_bks_2[] = {LSFT_T(KC_1), LCTL_T(KC_0), COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_ent_3_1, KC_ENT),
    COMBO(combo_ent_3_2, KC_ENT),
    COMBO(combo_esc_3_1, KC_ESC),
    COMBO(combo_esc_3_2, KC_ESC),
    COMBO(combo_tab_1, KC_TAB),
    COMBO(combo_tab_2, KC_TAB),
    COMBO(combo_bks_1, KC_BSPC),
    COMBO(combo_bks_2, KC_BSPC),
};

// }}} ----- COMBOS -----

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭────────────────────────────────────────────────────────────────────────Ъ────────╮
                KC_GRV,        KC_1,        KC_2,        KC_3,        KC_4,        KC_5,           KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_RBRC,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────────────────Х────────┤
                KC_QUOT,       KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,           KC_Y,    TD(TD_UND),   TD(TD_MIN),     KC_O,        KC_P,        KC_LBRC,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────────────────Э────────┤
                KC_BSLS,       KC_A,        KC_S, LCTL_T(KC_D),  LSFT_T(KC_F),  LGUI_T(KC_G),      KC_H,    LSFT_T(KC_J), LCTL_T(KC_K),   KC_L,     TD(TD_COL),     KC_QUOT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                PT_ESC,        KC_Z,        KC_X,        KC_C,    TD(TD_TIL),      KC_B,           KC_N,     TD(TD_PIP),    TD(TD_PON),   KC_DOT,      KC_SLSH,     PT_INS,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                      LCTL(KC_B),     KC_LALT, OSM(MOD_LGUI),      SYM,       NUM,
                                                                OSM(MOD_LCTL), OSM(MOD_LSFT),      NAV
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_SYM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                XXXXXXX,       KC_1,      KC_2,          KC_3,        KC_4,        KC_5,           KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,        KC_EXLM,     KC_LBRC,    TD(TD_BRC),     KC_EXLM,     KC_AT,       KC_ASTR,     KC_HASH,     _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_TILD,   XXXXXXX,        KC_PIPE,     KC_LPRN,    TD(TD_PAR),     KC_CIRC,     KC_PERC,     KC_DLR,      KC_UNDS,     KC_SCLN,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                _______,       KC_BSLS,   XXXXXXX,        KC_AMPR,     KC_LCBR,    TD(TD_CBR),     KC_LT,       KC_GT,       _______,     _______,     _______,     _______,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         TD(TD_GRV),  TD(TD_DQT),  TD(TD_QUT),     _______,     XXXXXXX,
                                                                      KC_BSLS,       KC_SLSH,      XXXXXXX
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                KC_F1,         KC_1,      KC_2,          KC_3,        KC_4,        KC_5,           KC_6,        KC_7,       KC_8,      KC_9,      KC_0,        XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_F2,         XXXXXXX,   XXXXXXX,       KC_LT,       KC_9,        XXXXXXX,        XXXXXXX,     KC_4,       KC_GT,     XXXXXXX,   _______,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_F3,         XXXXXXX,   KC_MINS,  LCTL_T(KC_6),  LSFT_T(KC_5),   _______,        KC_EQL, LSFT_T(KC_1), LCTL_T(KC_0), KC_PLUS,   _______,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_F4,         XXXXXXX,   XXXXXXX,       KC_7,        KC_8,        XXXXXXX,        KC_UNDS,        KC_2,       KC_3,      _______,   _______,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      CW_TOGG,     KC_SPC,         XXXXXXX,     _______,
                                                                      KC_DOT,      KC_0,           XXXXXXX
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     KC_VOLD,     KC_VOLU,        XXXXXXX,     XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       KC_UP,       KC_PGUP,     XXXXXXX,        XXXXXXX,     KC_HOME,    KC_PGUP,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_HOME,   KC_LEFT,       KC_DOWN,     KC_RGHT,     KC_END,         KC_LEFT,     KC_DOWN,    KC_UP,    KC_RGHT,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     KC_PGDN,     XXXXXXX,        XXXXXXX,     KC_END,     KC_PGDN,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      _______,     _______,        XXXXXXX,     XXXXXXX,
                                                                      _______,     _______,        _______  
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                QK_BOOT,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_SPDD,     UG_SPDU,        XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     QK_BOOT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_NEXT,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_VALD,     UG_VALU,        XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_PREV,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_HUED,     UG_HUEU,        KC_BTN2,        KC_BTN1,    KC_BTN3,  DRG_TOG,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_TOGG,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_SATD,     UG_SATU,        TO(LAYER_BASE), XXXXXXX,    _______,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      _______,     _______,        DRG_TOG,        DRG_TOG,
                                                                      _______,     _______,        SNP_TOG
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NUM2] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                XXXXXXX,       KC_F1,     KC_F2,         KC_F3,       KC_F4,       KC_F5,          XXXXXXX,     KC_7,       KC_8,     KC_9,      KC_0,        KC_MINS,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_F6,     KC_F7,         KC_F8,       KC_F9,       KC_F10,         XXXXXXX,     KC_7,       KC_8,     KC_9,      KC_UNDS,     KC_DQT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        KC_DOT,      KC_4,       KC_5,     KC_6,      _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        KC_0,        KC_1,       KC_2,     KC_3,      _______,     KC_BSLS,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      CW_TOGG,     KC_SPC,         XXXXXXX,     _______,
                                                                      KC_DOT,      KC_0,           XXXXXXX
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
