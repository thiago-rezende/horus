/* horus input layer [ windows ] */
#include <horus/platform/windows/input/keyboard.h>

#define PLATFORM_INPUT_KEYBOARD_KEYCODE_COUNT 256

static const keyboard_keycode_t platform_input_keyboard_keycode_mapping[PLATFORM_INPUT_KEYBOARD_KEYCODE_COUNT] = {
    [(u8)'A'] = KEYBOARD_KEYCODE_A,
    [(u8)'B'] = KEYBOARD_KEYCODE_B,
    [(u8)'C'] = KEYBOARD_KEYCODE_C,
    [(u8)'D'] = KEYBOARD_KEYCODE_D,
    [(u8)'E'] = KEYBOARD_KEYCODE_E,
    [(u8)'F'] = KEYBOARD_KEYCODE_F,
    [(u8)'G'] = KEYBOARD_KEYCODE_G,
    [(u8)'H'] = KEYBOARD_KEYCODE_H,
    [(u8)'I'] = KEYBOARD_KEYCODE_I,
    [(u8)'J'] = KEYBOARD_KEYCODE_J,
    [(u8)'K'] = KEYBOARD_KEYCODE_K,
    [(u8)'L'] = KEYBOARD_KEYCODE_L,
    [(u8)'M'] = KEYBOARD_KEYCODE_M,
    [(u8)'N'] = KEYBOARD_KEYCODE_N,
    [(u8)'O'] = KEYBOARD_KEYCODE_O,
    [(u8)'P'] = KEYBOARD_KEYCODE_P,
    [(u8)'Q'] = KEYBOARD_KEYCODE_Q,
    [(u8)'R'] = KEYBOARD_KEYCODE_R,
    [(u8)'S'] = KEYBOARD_KEYCODE_S,
    [(u8)'T'] = KEYBOARD_KEYCODE_T,
    [(u8)'U'] = KEYBOARD_KEYCODE_U,
    [(u8)'V'] = KEYBOARD_KEYCODE_V,
    [(u8)'W'] = KEYBOARD_KEYCODE_W,
    [(u8)'X'] = KEYBOARD_KEYCODE_X,
    [(u8)'Y'] = KEYBOARD_KEYCODE_Y,
    [(u8)'Z'] = KEYBOARD_KEYCODE_Z,

    /* numbers */
    [(u8)'0'] = KEYBOARD_KEYCODE_0,
    [(u8)'1'] = KEYBOARD_KEYCODE_1,
    [(u8)'2'] = KEYBOARD_KEYCODE_2,
    [(u8)'3'] = KEYBOARD_KEYCODE_3,
    [(u8)'4'] = KEYBOARD_KEYCODE_4,
    [(u8)'5'] = KEYBOARD_KEYCODE_5,
    [(u8)'6'] = KEYBOARD_KEYCODE_6,
    [(u8)'7'] = KEYBOARD_KEYCODE_7,
    [(u8)'8'] = KEYBOARD_KEYCODE_8,
    [(u8)'9'] = KEYBOARD_KEYCODE_9,

    /* general */
    [VK_TAB] = KEYBOARD_KEYCODE_TAB,
    [VK_RETURN] = KEYBOARD_KEYCODE_ENTER,
    [VK_SPACE] = KEYBOARD_KEYCODE_SPACE,
    [VK_ESCAPE] = KEYBOARD_KEYCODE_ESCAPE,
    [VK_BACK] = KEYBOARD_KEYCODE_BACKSPACE,

    /* arrows */
    [VK_UP] = KEYBOARD_KEYCODE_UP,
    [VK_DOWN] = KEYBOARD_KEYCODE_DOWN,
    [VK_LEFT] = KEYBOARD_KEYCODE_LEFT,
    [VK_RIGHT] = KEYBOARD_KEYCODE_RIGHT,

    /* modifiers */
    [VK_LMENU] = KEYBOARD_KEYCODE_LEFT_ALT,
    /* [0] = KEYBOARD_KEYCODE_LEFT_META, */
    [VK_RMENU] = KEYBOARD_KEYCODE_RIGHT_ALT,
    [VK_LSHIFT] = KEYBOARD_KEYCODE_LEFT_SHIFT,
    [VK_LWIN] = KEYBOARD_KEYCODE_LEFT_SUPER,
    /* [0] = KEYBOARD_KEYCODE_RIGHT_META, */
    [VK_RSHIFT] = KEYBOARD_KEYCODE_RIGHT_SHIFT,
    [VK_RWIN] = KEYBOARD_KEYCODE_RIGHT_SUPER,
    [VK_OEM_4] = KEYBOARD_KEYCODE_LEFT_BRACKET,
    [VK_CONTROL] = KEYBOARD_KEYCODE_LEFT_CONTROL,
    [VK_LCONTROL] = KEYBOARD_KEYCODE_LEFT_CONTROL,
    [VK_OEM_6] = KEYBOARD_KEYCODE_RIGHT_BRACKET,
    [VK_RCONTROL] = KEYBOARD_KEYCODE_RIGHT_CONTROL,

    /* functions */
    [VK_F1] = KEYBOARD_KEYCODE_F1,
    [VK_F2] = KEYBOARD_KEYCODE_F2,
    [VK_F3] = KEYBOARD_KEYCODE_F3,
    [VK_F4] = KEYBOARD_KEYCODE_F4,
    [VK_F5] = KEYBOARD_KEYCODE_F5,
    [VK_F6] = KEYBOARD_KEYCODE_F6,
    [VK_F7] = KEYBOARD_KEYCODE_F7,
    [VK_F8] = KEYBOARD_KEYCODE_F8,
    [VK_F9] = KEYBOARD_KEYCODE_F9,
    [VK_F10] = KEYBOARD_KEYCODE_F10,
    [VK_F11] = KEYBOARD_KEYCODE_F11,
    [VK_F12] = KEYBOARD_KEYCODE_F12,

    /* general */
    [VK_END] = KEYBOARD_KEYCODE_END,
    [VK_HOME] = KEYBOARD_KEYCODE_HOME,
    [VK_DELETE] = KEYBOARD_KEYCODE_DELETE,
    [VK_INSERT] = KEYBOARD_KEYCODE_INSERT,
    [VK_PRIOR] = KEYBOARD_KEYCODE_PAGEUP,
    [VK_NEXT] = KEYBOARD_KEYCODE_PAGEDOWN,

    /* general */
    [VK_NUMLOCK] = KEYBOARD_KEYCODE_NUMLOCK,
    [VK_CAPITAL] = KEYBOARD_KEYCODE_CAPSLOCK,
    [VK_SCROLL] = KEYBOARD_KEYCODE_SCROLLLOCK,

    /* general */
    [VK_PAUSE] = KEYBOARD_KEYCODE_PAUSE,
    [VK_SNAPSHOT] = KEYBOARD_KEYCODE_PRINTSCREEN,

    /* symbols */
    [VK_OEM_COMMA] = KEYBOARD_KEYCODE_COMMA,
    [VK_OEM_3] = KEYBOARD_KEYCODE_GRAVE,
    [VK_OEM_MINUS] = KEYBOARD_KEYCODE_MINUS,
    [VK_OEM_2] = KEYBOARD_KEYCODE_SLASH,
    [VK_OEM_PLUS] = KEYBOARD_KEYCODE_EQUALS,
    [VK_OEM_PERIOD] = KEYBOARD_KEYCODE_PERIOD,
    [VK_OEM_5] = KEYBOARD_KEYCODE_BACKSLASH,
    [VK_OEM_1] = KEYBOARD_KEYCODE_SEMICOLON,
    [VK_OEM_7] = KEYBOARD_KEYCODE_APOSTROPHE,
};

/* full 64K table, most entries will remain 0 (unmapped) */
static const keyboard_keycode_t platform_input_keyboard_scancode_mapping[0xFFFF] = {
    /* letters */
    [0x001E] = KEYBOARD_KEYCODE_A,
    [0x0030] = KEYBOARD_KEYCODE_B,
    [0x002E] = KEYBOARD_KEYCODE_C,
    [0x0020] = KEYBOARD_KEYCODE_D,
    [0x0012] = KEYBOARD_KEYCODE_E,
    [0x0021] = KEYBOARD_KEYCODE_F,
    [0x0022] = KEYBOARD_KEYCODE_G,
    [0x0023] = KEYBOARD_KEYCODE_H,
    [0x0017] = KEYBOARD_KEYCODE_I,
    [0x0024] = KEYBOARD_KEYCODE_J,
    [0x0025] = KEYBOARD_KEYCODE_K,
    [0x0026] = KEYBOARD_KEYCODE_L,
    [0x0032] = KEYBOARD_KEYCODE_M,
    [0x0031] = KEYBOARD_KEYCODE_N,
    [0x0018] = KEYBOARD_KEYCODE_O,
    [0x0019] = KEYBOARD_KEYCODE_P,
    [0x0010] = KEYBOARD_KEYCODE_Q,
    [0x0013] = KEYBOARD_KEYCODE_R,
    [0x001F] = KEYBOARD_KEYCODE_S,
    [0x0014] = KEYBOARD_KEYCODE_T,
    [0x0016] = KEYBOARD_KEYCODE_U,
    [0x002F] = KEYBOARD_KEYCODE_V,
    [0x0011] = KEYBOARD_KEYCODE_W,
    [0x002D] = KEYBOARD_KEYCODE_X,
    [0x0015] = KEYBOARD_KEYCODE_Y,
    [0x002C] = KEYBOARD_KEYCODE_Z,

    /* numbers */
    [0x000B] = KEYBOARD_KEYCODE_0,
    [0x0002] = KEYBOARD_KEYCODE_1,
    [0x0003] = KEYBOARD_KEYCODE_2,
    [0x0004] = KEYBOARD_KEYCODE_3,
    [0x0005] = KEYBOARD_KEYCODE_4,
    [0x0006] = KEYBOARD_KEYCODE_5,
    [0x0007] = KEYBOARD_KEYCODE_6,
    [0x0008] = KEYBOARD_KEYCODE_7,
    [0x0009] = KEYBOARD_KEYCODE_8,
    [0x000A] = KEYBOARD_KEYCODE_9,

    /* general */
    [0x000F] = KEYBOARD_KEYCODE_TAB,
    [0x001C] = KEYBOARD_KEYCODE_ENTER,
    [0x0039] = KEYBOARD_KEYCODE_SPACE,
    [0x0001] = KEYBOARD_KEYCODE_ESCAPE,
    [0x000E] = KEYBOARD_KEYCODE_BACKSPACE,

    /* arrows (E0-prefixed) */
    [0xE048] = KEYBOARD_KEYCODE_UP,
    [0xE050] = KEYBOARD_KEYCODE_DOWN,
    [0xE04B] = KEYBOARD_KEYCODE_LEFT,
    [0xE04D] = KEYBOARD_KEYCODE_RIGHT,

    /* modifiers */
    [0x0038] = KEYBOARD_KEYCODE_LEFT_ALT,
    /* [0x0000] = KEYBOARD_KEYCODE_LEFT_META, */
    [0xE038] = KEYBOARD_KEYCODE_RIGHT_ALT,
    [0x002A] = KEYBOARD_KEYCODE_LEFT_SHIFT,
    [0xE05B] = KEYBOARD_KEYCODE_LEFT_SUPER,
    /* [0x0000] = KEYBOARD_KEYCODE_RIGHT_META, */
    [0x0036] = KEYBOARD_KEYCODE_RIGHT_SHIFT,
    [0xE05C] = KEYBOARD_KEYCODE_RIGHT_SUPER,
    [0x001A] = KEYBOARD_KEYCODE_LEFT_BRACKET,
    [0x001D] = KEYBOARD_KEYCODE_LEFT_CONTROL,
    [0x001B] = KEYBOARD_KEYCODE_RIGHT_BRACKET,
    [0xE01D] = KEYBOARD_KEYCODE_RIGHT_CONTROL,

    /* functions */
    [0x003B] = KEYBOARD_KEYCODE_F1,
    [0x003C] = KEYBOARD_KEYCODE_F2,
    [0x003D] = KEYBOARD_KEYCODE_F3,
    [0x003E] = KEYBOARD_KEYCODE_F4,
    [0x003F] = KEYBOARD_KEYCODE_F5,
    [0x0040] = KEYBOARD_KEYCODE_F6,
    [0x0041] = KEYBOARD_KEYCODE_F7,
    [0x0042] = KEYBOARD_KEYCODE_F8,
    [0x0043] = KEYBOARD_KEYCODE_F9,
    [0x0044] = KEYBOARD_KEYCODE_F10,
    [0x0057] = KEYBOARD_KEYCODE_F11,
    [0x0058] = KEYBOARD_KEYCODE_F12,

    /* general */
    [0xE04F] = KEYBOARD_KEYCODE_END,
    [0xE047] = KEYBOARD_KEYCODE_HOME,
    [0xE053] = KEYBOARD_KEYCODE_DELETE,
    [0xE052] = KEYBOARD_KEYCODE_INSERT,
    [0xE049] = KEYBOARD_KEYCODE_PAGEUP,
    [0xE051] = KEYBOARD_KEYCODE_PAGEDOWN,

    /* general */
    [0x0045] = KEYBOARD_KEYCODE_NUMLOCK,
    [0x003A] = KEYBOARD_KEYCODE_CAPSLOCK,
    [0x0046] = KEYBOARD_KEYCODE_SCROLLLOCK,

    /* special */
    [0xE145] = KEYBOARD_KEYCODE_PAUSE,       /* Pause/Break (E1 1D 45 E1 9D C5) */
    [0xE037] = KEYBOARD_KEYCODE_PRINTSCREEN, /* PrintScreen (E0-prefixed) */

    /* symbols */
    [0x0033] = KEYBOARD_KEYCODE_COMMA,
    [0x0029] = KEYBOARD_KEYCODE_GRAVE,
    [0x000C] = KEYBOARD_KEYCODE_MINUS,
    [0x0035] = KEYBOARD_KEYCODE_SLASH,
    [0x000D] = KEYBOARD_KEYCODE_EQUALS,
    [0x0034] = KEYBOARD_KEYCODE_PERIOD,
    [0x002B] = KEYBOARD_KEYCODE_BACKSLASH,
    [0x0027] = KEYBOARD_KEYCODE_SEMICOLON,
    [0x0028] = KEYBOARD_KEYCODE_APOSTROPHE,
};

static keyboard_keycode_state_t __platform_input_keyboard_keycode_states[KEYBOARD_KEYCODE_COUNT] = {
    KEYBOARD_KEYCODE_STATE_NONE,
};

keyboard_keycode_t __platform_input_keyboard_keycode(WPARAM virtual_key) {
  return platform_input_keyboard_keycode_mapping[virtual_key];
}

keyboard_keycode_t __platform_input_keyboard_scancode(LPARAM parameters) {
  WORD flags = HIWORD(parameters);
  WORD scancode = LOBYTE(flags);
  BOOL is_extended_key = (flags & KF_EXTENDED) == KF_EXTENDED;

  if (is_extended_key) {
    scancode = MAKEWORD(scancode, 0xE0);
  }

  keyboard_keycode_t keycode = platform_input_keyboard_scancode_mapping[scancode];

  return keycode;
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
  keyboard_keycode_state_t state = __platform_input_keyboard_keycode_state(keycode);

  if (state == KEYBOARD_KEYCODE_STATE_PRESSED) {
    return true;
  }

  if (state == KEYBOARD_KEYCODE_STATE_HELD) {
    return true;
  }

  return false;
}

b8 platform_input_keyboard_keycode_is_released(keyboard_keycode_t keycode) {
  return __platform_input_keyboard_keycode_state(keycode) == KEYBOARD_KEYCODE_STATE_RELEASED;
}

keyboard_keycode_state_t platform_input_keyboard_keycode_state(keyboard_keycode_t keycode) {
  return __platform_input_keyboard_keycode_state(keycode);
}
