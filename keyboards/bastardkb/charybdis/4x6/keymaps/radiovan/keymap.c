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
    LAYER_EXTRA,
    LAYER_GAME
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
#define PT_ESC LT(LAYER_POINTER, KC_ESC)  // TODO deprecate, but add bootloader key
#define PT_INS LT(LAYER_POINTER, KC_INS)  // TODO deprecate
#define ML_L LT(LAYER_POINTER, KC_L)
#define EX_COMM LT(LAYER_EXTRA, KC_COMM)

// homerow shortcuts
#define HR_D LCTL_T(KC_D)
#define HR_F LSFT_T(KC_F)
#define HR_G LGUI_T(KC_G)

#define HR_K LCTL_T(KC_K)
#define HR_J LSFT_T(KC_J)


// {{{ ----- TAP DANCE -----

enum {
    TD_COL,
    TD_MIN,
    TD_PIP,
    TD_TIL,
    TD_UND,
    TD_SLS,

    TD_COM,

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
            SEND_STRING("(");
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
            SEND_STRING("[");
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
            SEND_STRING("{");
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
        case 3:
            SEND_STRING("```\n\n```"SS_TAP(X_UP));
            reset_tap_dance(state);
            break;
    }
}

void dance_min(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1:
            SEND_STRING("i");
            break;
        case 2:
            SEND_STRING("-");
            reset_tap_dance(state);
            break;
        case 3:
            SEND_STRING("--");
            reset_tap_dance(state);
            break;
    }
}

tap_dance_action_t tap_dance_actions[] = {
    [TD_COL] = ACTION_TAP_DANCE_DOUBLE(KC_SCLN, KC_COLN),
    [TD_PIP] = ACTION_TAP_DANCE_DOUBLE(KC_M, KC_PIPE),
    [TD_TIL] = ACTION_TAP_DANCE_DOUBLE(KC_V, KC_TILD),
    [TD_UND] = ACTION_TAP_DANCE_DOUBLE(KC_U, KC_UNDS),
    [TD_SLS] = ACTION_TAP_DANCE_DOUBLE(KC_SLSH, KC_QUES),

    [TD_COM] = ACTION_TAP_DANCE_LAYER_TOGGLE(KC_COMM, LAYER_POINTER),

    [TD_PAR] = ACTION_TAP_DANCE_FN(dance_par),
    [TD_BRC] = ACTION_TAP_DANCE_FN(dance_brc),
    [TD_CBR] = ACTION_TAP_DANCE_FN(dance_cbr),

    [TD_DQT] = ACTION_TAP_DANCE_FN(dance_dqt),
    [TD_QUT] = ACTION_TAP_DANCE_FN(dance_qut),
    [TD_GRV] = ACTION_TAP_DANCE_FN(dance_grv),

    [TD_MIN] = ACTION_TAP_DANCE_FN(dance_min)
};

// }}} ----- TAP DANCE -----


// {{{ ----- TAP HOLD -----

/*
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case METC:
            if (record->event.pressed) {
                // Action on hold
                MO(LAYER_EXTRA);
            } else {
                // Action on tap
                tap_code(KC_COMM);
            }
            return false;  // Prevent further processing
    }
    return true;  // Allow normal processing
}
*/

// }}} ----- TAP HOLD -----


// {{{ ----- COMBOS -----

const uint16_t PROGMEM combo_ent_3_1[] = {LSFT_T(KC_J), LCTL_T(KC_K), LT(LAYER_POINTER, KC_L), COMBO_END};
const uint16_t PROGMEM combo_ent_3_2[] = {LSFT_T(KC_1), LCTL_T(KC_0), KC_PLUS, COMBO_END};
const uint16_t PROGMEM combo_ent_3_3[] = {KC_PERC, KC_DLR, KC_PLUS, COMBO_END};

const uint16_t PROGMEM combo_esc_3_1[] = {KC_S, LCTL_T(KC_D), LSFT_T(KC_F), COMBO_END};
const uint16_t PROGMEM combo_esc_3_2[] = {KC_MINS, LCTL_T(KC_6), LSFT_T(KC_5), COMBO_END};
const uint16_t PROGMEM combo_esc_3_3[] = {KC_MINS, KC_PIPE, TD(TD_PAR), COMBO_END};

const uint16_t PROGMEM combo_tab_1[] = {LCTL_T(KC_D), LSFT_T(KC_F), COMBO_END};
const uint16_t PROGMEM combo_tab_2[] = {LCTL_T(KC_6), LSFT_T(KC_5), COMBO_END};
const uint16_t PROGMEM combo_tab_3[] = {KC_PIPE, TD(TD_PAR), COMBO_END};

const uint16_t PROGMEM combo_bks_1[] = {LSFT_T(KC_J), LCTL_T(KC_K), COMBO_END};
const uint16_t PROGMEM combo_bks_2[] = {LSFT_T(KC_1), LCTL_T(KC_0), COMBO_END};
const uint16_t PROGMEM combo_bks_3[] = {KC_PERC, KC_DLR, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_ent_3_1, KC_ENT),
    COMBO(combo_ent_3_2, KC_ENT),
    COMBO(combo_ent_3_3, KC_ENT),

    COMBO(combo_esc_3_1, KC_ESC),
    COMBO(combo_esc_3_2, KC_ESC),
    COMBO(combo_esc_3_3, KC_ESC),

    COMBO(combo_tab_1, KC_TAB),
    COMBO(combo_tab_2, KC_TAB),
    COMBO(combo_tab_3, KC_TAB),
    
    COMBO(combo_bks_1, KC_BSPC),
    COMBO(combo_bks_2, KC_BSPC),
    COMBO(combo_bks_3, KC_BSPC)
};

// }}} ----- COMBOS -----


enum custom_keys { KC_PRINT_HUE = SAFE_RANGE };

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    switch (keycode) {
    case KC_PRINT_HUE: {
#ifdef RGB_MATRIX_ENABLE
        // rgb_matrix_get_hue() returns uint8_t [0..255]
        uint8_t hue = rgb_matrix_get_hue();

        // Build string and send
        char buf[16];
        snprintf(buf, sizeof(buf), "HUE: %d", hue);
        SEND_STRING(buf);
#else
        SEND_STRING("RGB Matrix not enabled");
#endif
        return false; // don't send keycode further
    }
    }
    return true;
}


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭────────────────────────────────────────────────────────────────────────Ъ────────╮
                QK_LLCK,       KC_1,        KC_2,        KC_3,        KC_4,        KC_5,           KC_6,        KC_7,        KC_8,         KC_9,        KC_0,        KC_RBRC,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────────────────Х────────┤
                KC_VOLU,       KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,           KC_Y,    TD(TD_UND),   TD(TD_MIN),      KC_O,        KC_P,        KC_LBRC,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├────────────────────────────────────────────────────────────────────────Э────────┤
                KC_VOLD,       KC_A,        KC_S,        HR_D,        HR_F,        HR_G,           KC_H,      HR_J,          HR_K,         ML_L,     TD(TD_COL),     KC_QUOT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                PT_ESC,        KC_Z,        KC_X,        KC_C,    TD(TD_TIL),      KC_B,           KC_N,     TD(TD_PIP),     EX_COMM,      KC_DOT,   TD(TD_SLS),     PT_INS,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                      LCTL(KC_B),     KC_LALT, OSM(MOD_LGUI),      SYM,       NUM,
                                                                OSM(MOD_LCTL), OSM(MOD_LSFT),      NAV
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_SYM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                _______,       KC_1,      KC_2,          KC_3,        KC_4,        KC_5,           KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       KC_EXLM,     TD(TD_CBR),  KC_RCBR,        KC_AMPR,     KC_AT,       KC_ASTR,     KC_HASH,     _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_TILD,   KC_MINS,       KC_PIPE,     TD(TD_PAR),  KC_RPRN,        KC_CIRC,     KC_PERC,     KC_DLR,      KC_PLUS,     _______,     _______,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                _______,       KC_BSLS,   XXXXXXX,       KC_UNDS,     TD(TD_BRC),  KC_RBRC,        KC_LT,       KC_GT,       _______,     _______,     _______,     _______,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         TD(TD_GRV),  TD(TD_DQT),  TD(TD_QUT),     _______,     XXXXXXX,
                                                                      KC_BSLS,       KC_SLSH,      XXXXXXX
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NUM] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                _______,        KC_F1,     KC_F2,         KC_F3,       KC_F4,       KC_F5,          KC_F6,       KC_F7,      KC_F8,     KC_F9,     KC_F10,      XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_F2,         XXXXXXX,   XXXXXXX,       KC_LT,       KC_9,        XXXXXXX,        XXXXXXX,     KC_4,       KC_GT,     XXXXXXX,   _______,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_F3,         XXXXXXX,   KC_MINS,  LCTL_T(KC_6),  LSFT_T(KC_5),   _______,        KC_EQL, LSFT_T(KC_1), LCTL_T(KC_0), KC_PLUS,   _______,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_F4,         XXXXXXX,   XXXXXXX,       KC_7,        KC_8,        XXXXXXX,        KC_UNDS,     KC_2,       KC_3,      _______,   _______,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         TD(TD_GRV),  TD(TD_DQT),  TD(TD_QUT),     XXXXXXX,     _______,
                                                                      KC_BSLS,       KC_SLSH,      XXXXXXX
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                _______,       XXXXXXX,   XXXXXXX,       XXXXXXX,     KC_VOLD,     KC_VOLU,        XXXXXXX,     XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
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
                _______,       MS_ACL0,   MS_ACL1,       MS_ACL2,     XXXXXXX,     XXXXXXX,        XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       KC_UP,       XXXXXXX,     XXXXXXX,        XXXXXXX,        MS_WHLU,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_HOME,   KC_LEFT,       KC_DOWN,     KC_RIGHT,    KC_END,         KC_BTN2,        KC_BTN1,    KC_BTN3,  _______,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   XXXXXXX,       XXXXXXX,     XXXXXXX,     XXXXXXX,        XXXXXXX,        MS_WHLD,    _______,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      _______,     _______,        SNP_TOG,        DRG_TOG,
                                                                      _______,     _______,        QK_LLCK
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  [LAYER_EXTRA] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                QK_BOOT,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_SPDD,     UG_SPDU,        XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     QK_BOOT,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_PRINT_HUE,  XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_VALD,     UG_VALU,        XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_PREV,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_HUED, TO(LAYER_GAME),     XXXXXXX,        XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                UG_TOGG,       XXXXXXX,   XXXXXXX,       XXXXXXX,     UG_SATD,     UG_SATU,        XXXXXXX,        XXXXXXX,    _______,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      UG_VALD,     UG_VALU,        UG_PREV,        UG_NEXT,
                                                                      UG_SPDD,     UG_SPDU,        UG_TOGG
  //                                       ╰────────────────────────────────────────────────╯ ╰──────────────────────────────────────────────╯
  ),

  // TODO
  // - fix three key combo
  // - control brightness on layer color remap

  [LAYER_GAME] = LAYOUT(
  // ╭──────────────────────────────────────────────────────────────────────────────────────╮ ╭─────────────────────────────────────────────────────────────────────────────────╮
                XXXXXXX,       KC_1,      KC_2,          KC_3,        KC_4,        KC_5,           XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,     TO(LAYER_BASE),
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       XXXXXXX,   KC_Q,          KC_W,        KC_E,        KC_R,           KC_Y,      KC_U,      KC_I,     KC_O,      KC_P,        XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                XXXXXXX,       KC_TAB,    KC_A,          KC_S,        KC_D,        KC_F,           KC_H,      KC_BTN1,   KC_BTN2,  KC_BTN3,   XXXXXXX,     XXXXXXX,
  // ├──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────┤
                KC_ESC,        KC_X,      KC_Z,          KC_C,        KC_V,        KC_G,           KC_N,      KC_M,      XXXXXXX,  XXXXXXX,   XXXXXXX,     XXXXXXX,
  // ╰──────────────────────────────────────────────────────────────────────────────────────┤ ├─────────────────────────────────────────────────────────────────────────────────╯
                                                         QK_REP,      KC_LALT,     MS_BTN4,        MS_WHLU,   SNP_TOG,
                                                                      KC_LCTL,     KC_LSFT,        MS_WHLD
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

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (get_highest_layer(layer_state) > 0) {
        uint8_t layer = get_highest_layer(layer_state);

        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];

                if (index >= led_min && index < led_max && index != NO_LED) {
                    uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                    switch(layer) {
                        case LAYER_GAME:
                            if (keycode == KC_W) {
                                rgb_matrix_set_color(index, RGB_RED);
                            } else if (keycode == KC_A) {
                                rgb_matrix_set_color(index, RGB_RED);
                            } else if (keycode == KC_S) {
                                rgb_matrix_set_color(index, RGB_RED);
                            } else if (keycode == KC_D) {
                                rgb_matrix_set_color(index, RGB_RED);
                            } else if (keycode == TO(LAYER_BASE)) {
                                rgb_matrix_set_color(index, RGB_YELLOW);
                            }
                            if (keycode == KC_BTN1 || keycode == KC_BTN2 || keycode == KC_BTN3 || keycode == KC_BTN4) {
                                rgb_matrix_set_color(index, RGB_ORANGE);
                            }
                            break;
                        case LAYER_SYM:
                            if (keycode == LT(LAYER_SYM, KC_SPC)) {
                                rgb_matrix_set_color(index, RGB_ORANGE);
                            } else if (keycode == QK_LLCK) {
                                rgb_matrix_set_color(index, RGB_RED);
                            }
                            rgb_matrix_set_color(1, RGB_RED);
                            break;
                        case LAYER_NUM:
                            if (keycode == LCTL_T(KC_0)) {
                                rgb_matrix_set_color(index, RGB_WHITE);
                            } else if (keycode == LSFT_T(KC_1)) {
                                rgb_matrix_set_color(index, RGB_RED);
                            } else if (keycode == KC_2) {
                                rgb_matrix_set_color(index, RGB_ORANGE);
                            } else if (keycode == KC_3) {
                                rgb_matrix_set_color(index, RGB_YELLOW);
                            } else if (keycode == KC_4) {
                                rgb_matrix_set_color(index, RGB_GREEN);
                            } else if (keycode == KC_5) {
                                rgb_matrix_set_color(index, RGB_TEAL);
                            } else if (keycode == KC_6) {
                                rgb_matrix_set_color(index, RGB_BLUE);
                            } else if (keycode == KC_7) {
                                rgb_matrix_set_color(index, RGB_MAGENTA);
                            } else if (keycode == KC_8) {
                                rgb_matrix_set_color(index, RGB_TURQUOISE);
                            } else if (keycode == KC_9) {
                                rgb_matrix_set_color(index, RGB_CYAN);
                            } else if (keycode == QK_LLCK) {
                                rgb_matrix_set_color(index, RGB_RED);
                            }
                            break;
                        case LAYER_NAV:
                            if (keycode == KC_LEFT) {
                                rgb_matrix_set_color(index, RGB_TEAL);
                            } else if (keycode == KC_RIGHT) {
                                rgb_matrix_set_color(index, RGB_TEAL);
                            } else if (keycode == KC_UP) {
                                rgb_matrix_set_color(index, RGB_TEAL);
                            } else if (keycode == KC_DOWN) {
                                rgb_matrix_set_color(index, RGB_TEAL);
                            } else if (keycode == KC_HOME) {
                                rgb_matrix_set_color(index, RGB_CYAN);
                            } else if (keycode == KC_END) {
                                rgb_matrix_set_color(index, RGB_CYAN);
                            } else if (keycode == KC_PGUP) {
                                rgb_matrix_set_color(index, RGB_SPRINGGREEN);
                            } else if (keycode == KC_PGDN) {
                                rgb_matrix_set_color(index, RGB_SPRINGGREEN);
                            } else if (keycode == QK_REP) {
                                rgb_matrix_set_color(index, RGB_ORANGE);
                            } else if (keycode == QK_LLCK) {
                                rgb_matrix_set_color(index, RGB_RED);
                            }
                            break;
                        case LAYER_EXTRA:
                            if (keycode == KC_COMM) {
                                rgb_matrix_set_color(index, RGB_ORANGE);
                            } else if (keycode == QK_BOOT) {
                                rgb_matrix_set_color(index, RGB_RED);
                            } else if (keycode == QK_REP) {
                                rgb_matrix_set_color(index, RGB_ORANGE);
                            } else if (keycode > KC_TRNS) {
                                rgb_matrix_set_color(index, RGB_TEAL);
                            }
                            break;
                        case LAYER_POINTER:
                            if (charybdis_get_pointer_dragscroll_enabled()) {
                                if (keycode == KC_BTN1 || keycode == KC_BTN2 || keycode == KC_BTN3) {
                                    rgb_matrix_set_color(index, RGB_ORANGE);
                                }
                            } else {
                                if (keycode == KC_BTN1 || keycode == KC_BTN2 || keycode == KC_BTN3) {
                                    rgb_matrix_set_color(index, RGB_TEAL);
                                }
                            }
                            if (keycode == QK_LLCK) {
                                rgb_matrix_set_color(index, RGB_RED);
                            }
                            break;
                    }
                }
            }
        }
    }
    return false;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case LAYER_POINTER:
        charybdis_set_pointer_dragscroll_enabled(0);
        break;
    case LAYER_GAME:
        charybdis_set_pointer_dragscroll_enabled(0);
        break;
    default:
        charybdis_set_pointer_dragscroll_enabled(1);
        break;
    }
  return state;
}
