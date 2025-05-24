/* horus input layer */
#include <horus/input/keyboard.h>

/* horus platform layer */
#include <horus/platform/detection.h>

#if defined(HORUS_PLATFORM_LINUX)

#include <horus/platform/linux/input/keyboard.h>

#endif

static const char *keyboard_keycode_strings[KEYBOARD_KEYCODE_COUNT] = {
    [KEYBOARD_KEYCODE_NONE] = "none",

    /* letters */
    [KEYBOARD_KEYCODE_A] = "a",
    [KEYBOARD_KEYCODE_B] = "b",
    [KEYBOARD_KEYCODE_C] = "c",
    [KEYBOARD_KEYCODE_D] = "d",
    [KEYBOARD_KEYCODE_E] = "e",
    [KEYBOARD_KEYCODE_F] = "f",
    [KEYBOARD_KEYCODE_G] = "g",
    [KEYBOARD_KEYCODE_H] = "h",
    [KEYBOARD_KEYCODE_I] = "i",
    [KEYBOARD_KEYCODE_J] = "j",
    [KEYBOARD_KEYCODE_K] = "k",
    [KEYBOARD_KEYCODE_L] = "l",
    [KEYBOARD_KEYCODE_M] = "m",
    [KEYBOARD_KEYCODE_N] = "n",
    [KEYBOARD_KEYCODE_O] = "o",
    [KEYBOARD_KEYCODE_P] = "p",
    [KEYBOARD_KEYCODE_Q] = "q",
    [KEYBOARD_KEYCODE_R] = "r",
    [KEYBOARD_KEYCODE_S] = "s",
    [KEYBOARD_KEYCODE_T] = "t",
    [KEYBOARD_KEYCODE_U] = "u",
    [KEYBOARD_KEYCODE_V] = "v",
    [KEYBOARD_KEYCODE_W] = "w",
    [KEYBOARD_KEYCODE_X] = "x",
    [KEYBOARD_KEYCODE_Y] = "y",
    [KEYBOARD_KEYCODE_Z] = "z",

    /* numbers */
    [KEYBOARD_KEYCODE_0] = "0",
    [KEYBOARD_KEYCODE_1] = "1",
    [KEYBOARD_KEYCODE_2] = "2",
    [KEYBOARD_KEYCODE_3] = "3",
    [KEYBOARD_KEYCODE_4] = "4",
    [KEYBOARD_KEYCODE_5] = "5",
    [KEYBOARD_KEYCODE_6] = "6",
    [KEYBOARD_KEYCODE_7] = "7",
    [KEYBOARD_KEYCODE_8] = "8",
    [KEYBOARD_KEYCODE_9] = "9",

    /* general */
    [KEYBOARD_KEYCODE_TAB] = "tab",
    [KEYBOARD_KEYCODE_ENTER] = "enter",
    [KEYBOARD_KEYCODE_SPACE] = "space",
    [KEYBOARD_KEYCODE_ESCAPE] = "escape",
    [KEYBOARD_KEYCODE_BACKSPACE] = "backspace",

    /* arrows */
    [KEYBOARD_KEYCODE_UP] = "up",
    [KEYBOARD_KEYCODE_DOWN] = "down",
    [KEYBOARD_KEYCODE_LEFT] = "left",
    [KEYBOARD_KEYCODE_RIGHT] = "right",

    /* modifiers */
    [KEYBOARD_KEYCODE_PRINT] = "print",
    [KEYBOARD_KEYCODE_PAGE_UP] = "page_up",
    [KEYBOARD_KEYCODE_LEFT_ALT] = "left_alt",
    [KEYBOARD_KEYCODE_NUM_LOCK] = "num_lock",
    [KEYBOARD_KEYCODE_CAPS_LOCK] = "caps_lock",
    [KEYBOARD_KEYCODE_LEFT_META] = "left_meta",
    [KEYBOARD_KEYCODE_PAGE_DOWN] = "page_down",
    [KEYBOARD_KEYCODE_RIGHT_ALT] = "right_alt",
    [KEYBOARD_KEYCODE_LEFT_SHIFT] = "left_shift",
    [KEYBOARD_KEYCODE_LEFT_SUPER] = "left_super",
    [KEYBOARD_KEYCODE_RIGHT_META] = "right_meta",
    [KEYBOARD_KEYCODE_RIGHT_SHIFT] = "right_shift",
    [KEYBOARD_KEYCODE_RIGHT_SUPER] = "right_super",
    [KEYBOARD_KEYCODE_SCROLL_LOCK] = "scroll_lock",
    [KEYBOARD_KEYCODE_LEFT_BRACKET] = "left_bracket",
    [KEYBOARD_KEYCODE_LEFT_CONTROL] = "left_control",
    [KEYBOARD_KEYCODE_RIGHT_BRACKET] = "right_bracket",
    [KEYBOARD_KEYCODE_RIGHT_CONTROL] = "right_control",

    /* functions */
    [KEYBOARD_KEYCODE_F1] = "f1",
    [KEYBOARD_KEYCODE_F2] = "f2",
    [KEYBOARD_KEYCODE_F3] = "f3",
    [KEYBOARD_KEYCODE_F4] = "f4",
    [KEYBOARD_KEYCODE_F5] = "f5",
    [KEYBOARD_KEYCODE_F6] = "f6",
    [KEYBOARD_KEYCODE_F7] = "f7",
    [KEYBOARD_KEYCODE_F8] = "f8",
    [KEYBOARD_KEYCODE_F9] = "f9",
    [KEYBOARD_KEYCODE_F10] = "f10",
    [KEYBOARD_KEYCODE_F11] = "f11",
    [KEYBOARD_KEYCODE_F12] = "f12",

    /* general */
    [KEYBOARD_KEYCODE_END] = "end",
    [KEYBOARD_KEYCODE_HOME] = "home",
    [KEYBOARD_KEYCODE_DELETE] = "delete",
    [KEYBOARD_KEYCODE_INSERT] = "insert",
    [KEYBOARD_KEYCODE_PAGEUP] = "pageup",
    [KEYBOARD_KEYCODE_PAGEDOWN] = "pagedown",

    /* general */
    [KEYBOARD_KEYCODE_NUMLOCK] = "numlock",
    [KEYBOARD_KEYCODE_CAPSLOCK] = "capslock",
    [KEYBOARD_KEYCODE_SCROLLLOCK] = "scrolllock",

    /* general */
    [KEYBOARD_KEYCODE_PAUSE] = "pause",
    [KEYBOARD_KEYCODE_PRINTSCREEN] = "printscreen",

    /* symbols */
    [KEYBOARD_KEYCODE_COMMA] = "comma",
    [KEYBOARD_KEYCODE_GRAVE] = "grave",
    [KEYBOARD_KEYCODE_MINUS] = "minus",
    [KEYBOARD_KEYCODE_SLASH] = "slash",
    [KEYBOARD_KEYCODE_EQUALS] = "equals",
    [KEYBOARD_KEYCODE_PERIOD] = "period",
    [KEYBOARD_KEYCODE_BACKSLASH] = "backslash",
    [KEYBOARD_KEYCODE_SEMICOLON] = "semicolon",
    [KEYBOARD_KEYCODE_APOSTROPHE] = "apostrophe",
};

const char *keyboard_keycode_state_strings[KEYBOARD_KEYCODE_STATE_COUNT] = {
    [KEYBOARD_KEYCODE_STATE_NONE] = "none",
    [KEYBOARD_KEYCODE_STATE_HELD] = "held",
    [KEYBOARD_KEYCODE_STATE_PRESSED] = "pressed",
    [KEYBOARD_KEYCODE_STATE_RELEASED] = "released",
};

const char *input_keyboard_keycode_string(keyboard_keycode_t keycode) {
  if (keycode < KEYBOARD_KEYCODE_COUNT && keycode >= KEYBOARD_KEYCODE_NONE) {
    return keyboard_keycode_strings[keycode];
  }

  return "unknown";
}

const char *input_keyboard_keycode_state_string(keyboard_keycode_state_t state) {
  if (state < KEYBOARD_KEYCODE_STATE_COUNT && state >= KEYBOARD_KEYCODE_STATE_NONE) {
    return keyboard_keycode_state_strings[state];
  }

  return "unknown";
}

b8 input_keyboard_keycode_is_held(keyboard_keycode_t keycode) {
  return platform_input_keyboard_keycode_is_held(keycode);
}

b8 input_keyboard_keycode_is_pressed(keyboard_keycode_t keycode) {
  return platform_input_keyboard_keycode_is_pressed(keycode);
}

b8 input_keyboard_keycode_is_released(keyboard_keycode_t keycode) {
  return platform_input_keyboard_keycode_is_released(keycode);
}

keyboard_keycode_state_t input_keyboard_keycode_state(keyboard_keycode_t keycode) {
  return platform_input_keyboard_keycode_state(keycode);
}
