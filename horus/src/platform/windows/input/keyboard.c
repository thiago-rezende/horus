/* horus input layer [ windows ] */
#include <horus/platform/windows/input/keyboard.h>

#define PLATFORM_INPUT_KEYBOARD_KEYCODE_COUNT 256

static const keyboard_keycode_t platform_input_keyboard_keycode_mapping[PLATFORM_INPUT_KEYBOARD_KEYCODE_COUNT] = {
    KEYBOARD_KEYCODE_NONE,
};

static keyboard_keycode_state_t __platform_input_keyboard_keycode_states[KEYBOARD_KEYCODE_COUNT] = {
    KEYBOARD_KEYCODE_STATE_NONE,
};

keyboard_keycode_t __platform_input_keyboard_keycode(/* platform_keycode_t */ u8 keycode) {
  return platform_input_keyboard_keycode_mapping[keycode];
}

b8 __platform_input_keyboard_keycode_clear_state(void) {
  for (keyboard_keycode_t keycode = KEYBOARD_KEYCODE_NONE; keycode < KEYBOARD_KEYCODE_COUNT; ++keycode) {
    if (__platform_input_keyboard_keycode_states[keycode] == KEYBOARD_KEYCODE_STATE_RELEASED) {
      __platform_input_keyboard_keycode_states[keycode] = KEYBOARD_KEYCODE_STATE_NONE;
    }
  }

  return true;
}

b8 __platform_input_keyboard_keycode_set_state(keyboard_keycode_t keycode, keyboard_keycode_state_t state) {
  if (keycode < KEYBOARD_KEYCODE_COUNT && keycode >= KEYBOARD_KEYCODE_NONE) {
    __platform_input_keyboard_keycode_states[keycode] = state;

    return true;
  }

  return false;
}

keyboard_keycode_state_t __platform_input_keyboard_keycode_state(keyboard_keycode_t keycode) {
  if (keycode < KEYBOARD_KEYCODE_COUNT && keycode >= KEYBOARD_KEYCODE_NONE) {
    return __platform_input_keyboard_keycode_states[keycode];
  }

  return KEYBOARD_KEYCODE_STATE_NONE;
}

keyboard_keycode_state_t __platform_input_keyboard_keycode_pressed_state(keyboard_keycode_t keycode) {
  keyboard_keycode_state_t previous = __platform_input_keyboard_keycode_state(keycode);

  if (previous == KEYBOARD_KEYCODE_STATE_PRESSED || previous == KEYBOARD_KEYCODE_STATE_HELD) {
    return KEYBOARD_KEYCODE_STATE_HELD;
  }

  return KEYBOARD_KEYCODE_STATE_PRESSED;
}

b8 platform_input_keyboard_keycode_is_held(keyboard_keycode_t keycode) {
  return __platform_input_keyboard_keycode_state(keycode) == KEYBOARD_KEYCODE_STATE_HELD;
}

b8 platform_input_keyboard_keycode_is_pressed(keyboard_keycode_t keycode) {
  return __platform_input_keyboard_keycode_state(keycode) == KEYBOARD_KEYCODE_STATE_PRESSED;
}

b8 platform_input_keyboard_keycode_is_released(keyboard_keycode_t keycode) {
  return __platform_input_keyboard_keycode_state(keycode) == KEYBOARD_KEYCODE_STATE_RELEASED;
}

keyboard_keycode_state_t platform_input_keyboard_keycode_state(keyboard_keycode_t keycode) {
  return __platform_input_keyboard_keycode_state(keycode);
}
