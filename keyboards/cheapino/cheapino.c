#include "wait.h"
#include "quantum.h"

// This is to keep state between callbacks, when it is 0 the
// initial RGB flash is finished
uint8_t _hue_countdown = 50;

// Do a little 2.5 seconds display of the different colors
// Use the deferred executor so the LED flash dance does not
// stop us from using the keyboard.
// https://docs.qmk.fm/#/custom_quantum_functions?id=deferred-executor-registration
uint32_t flash_led(uint32_t next_trigger_time, void *cb_arg) {
    rgblight_sethsv(_hue_countdown * 5, 230, 70);
    _hue_countdown--;
    if (_hue_countdown == 0) {
        // Finished, reset to user chosen led color
        rgblight_sethsv(
                RGBLIGHT_DEFAULT_HUE,
                RGBLIGHT_DEFAULT_SAT,
                RGBLIGHT_DEFAULT_VAL
        );
        return 0;
    } else {
        return 50;
    }
}

void keyboard_post_init_user(void) {
    //debug_enable=true;
    //debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;

    // Flash a little on start
    defer_exec(50, flash_led, NULL);
}

// Make the builtin RGB led show different colors per layer
uint8_t get_hue(uint8_t layer) {
    switch (layer) {
        case 6:
            return 201;
        case 5:
            return 43;
        case 4:
            return 127;
        case 7:
            return 169;
        case 8:
            return 85;
        case 9:
            return 0;
        default:
            return RGBLIGHT_DEFAULT_HUE;
    }
}

// Required to display white
uint8_t get_sat(uint8_t layer) {
    switch (layer) {
        case 9:
        case 8:
        case 7:
        case 6:
        case 5:
        case 4:
            return 230;
        default:
            return RGBLIGHT_DEFAULT_HUE;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t val = RGBLIGHT_DEFAULT_VAL;
    uint8_t sat = get_sat(get_highest_layer(state));
    uint8_t hue = get_hue(get_highest_layer(state));
    rgblight_sethsv(hue, sat, val);
    return state;
}
