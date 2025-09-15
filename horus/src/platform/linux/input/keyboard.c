/* horus input layer [ linux ] */
#include <horus/platform/linux/input/keyboard.h>

#include <X11/keysym.h>

#define PLATFORM_INPUT_KEYBOARD_KEYCODE_COUNT 256

static const keyboard_keycode_t platform_input_keyboard_keycode_mapping[PLATFORM_INPUT_KEYBOARD_KEYCODE_COUNT] = {
    [0] = KEYBOARD_KEYCODE_NONE,            /* Reserved */
    [1] = KEYBOARD_KEYCODE_NONE,            /* Reserved */
    [2] = KEYBOARD_KEYCODE_NONE,            /* Reserved */
    [3] = KEYBOARD_KEYCODE_NONE,            /* Cancel */
    [4] = KEYBOARD_KEYCODE_NONE,            /* Reserved */
    [5] = KEYBOARD_KEYCODE_NONE,            /* Reserved */
    [6] = KEYBOARD_KEYCODE_NONE,            /* Help */
    [7] = KEYBOARD_KEYCODE_NONE,            /* Reserved */
    [8] = KEYBOARD_KEYCODE_BACKSPACE,       /* Backspace */
    [9] = KEYBOARD_KEYCODE_ESCAPE,          /* Escape */
    [10] = KEYBOARD_KEYCODE_1,              /* 1 */
    [11] = KEYBOARD_KEYCODE_2,              /* 2 */
    [12] = KEYBOARD_KEYCODE_3,              /* 3 */
    [13] = KEYBOARD_KEYCODE_4,              /* 4 */
    [14] = KEYBOARD_KEYCODE_5,              /* 5 */
    [15] = KEYBOARD_KEYCODE_6,              /* 6 */
    [16] = KEYBOARD_KEYCODE_7,              /* 7 */
    [17] = KEYBOARD_KEYCODE_8,              /* 8 */
    [18] = KEYBOARD_KEYCODE_9,              /* 9 */
    [19] = KEYBOARD_KEYCODE_0,              /* 0 */
    [20] = KEYBOARD_KEYCODE_MINUS,          /* - */
    [21] = KEYBOARD_KEYCODE_EQUALS,         /* = */
    [22] = KEYBOARD_KEYCODE_BACKSPACE,      /* Backspace */
    [23] = KEYBOARD_KEYCODE_TAB,            /* Tab */
    [24] = KEYBOARD_KEYCODE_Q,              /* q */
    [25] = KEYBOARD_KEYCODE_W,              /* w */
    [26] = KEYBOARD_KEYCODE_E,              /* e */
    [27] = KEYBOARD_KEYCODE_R,              /* r */
    [28] = KEYBOARD_KEYCODE_T,              /* t */
    [29] = KEYBOARD_KEYCODE_Y,              /* y */
    [30] = KEYBOARD_KEYCODE_U,              /* u */
    [31] = KEYBOARD_KEYCODE_I,              /* i */
    [32] = KEYBOARD_KEYCODE_O,              /* o */
    [33] = KEYBOARD_KEYCODE_P,              /* p */
    [34] = KEYBOARD_KEYCODE_LEFT_BRACKET,   /* [ */
    [35] = KEYBOARD_KEYCODE_RIGHT_BRACKET,  /* ] */
    [36] = KEYBOARD_KEYCODE_ENTER,          /* Enter */
    [37] = KEYBOARD_KEYCODE_LEFT_CONTROL,   /* Left Control */
    [38] = KEYBOARD_KEYCODE_A,              /* a */
    [39] = KEYBOARD_KEYCODE_S,              /* s */
    [40] = KEYBOARD_KEYCODE_D,              /* d */
    [41] = KEYBOARD_KEYCODE_F,              /* f */
    [42] = KEYBOARD_KEYCODE_G,              /* g */
    [43] = KEYBOARD_KEYCODE_H,              /* h */
    [44] = KEYBOARD_KEYCODE_J,              /* j */
    [45] = KEYBOARD_KEYCODE_K,              /* k */
    [46] = KEYBOARD_KEYCODE_L,              /* l */
    [47] = KEYBOARD_KEYCODE_SEMICOLON,      /* ; */
    [48] = KEYBOARD_KEYCODE_APOSTROPHE,     /* ' */
    [49] = KEYBOARD_KEYCODE_GRAVE,          /* ` */
    [50] = KEYBOARD_KEYCODE_LEFT_SHIFT,     /* Left Shift */
    [51] = KEYBOARD_KEYCODE_BACKSLASH,      /* \ */
    [52] = KEYBOARD_KEYCODE_Z,              /* z */
    [53] = KEYBOARD_KEYCODE_X,              /* x */
    [54] = KEYBOARD_KEYCODE_C,              /* c */
    [55] = KEYBOARD_KEYCODE_V,              /* v */
    [56] = KEYBOARD_KEYCODE_B,              /* b */
    [57] = KEYBOARD_KEYCODE_N,              /* n */
    [58] = KEYBOARD_KEYCODE_M,              /* m */
    [59] = KEYBOARD_KEYCODE_COMMA,          /* , */
    [60] = KEYBOARD_KEYCODE_PERIOD,         /* . */
    [61] = KEYBOARD_KEYCODE_SLASH,          /* / */
    [62] = KEYBOARD_KEYCODE_RIGHT_SHIFT,    /* Right Shift */
    [63] = KEYBOARD_KEYCODE_NONE,           /* Keypad *  */
    [64] = KEYBOARD_KEYCODE_LEFT_ALT,       /* Left Alt */
    [65] = KEYBOARD_KEYCODE_SPACE,          /* Space */
    [66] = KEYBOARD_KEYCODE_CAPSLOCK,       /* Caps Lock */
    [67] = KEYBOARD_KEYCODE_F1,             /* F1 */
    [68] = KEYBOARD_KEYCODE_F2,             /* F2 */
    [69] = KEYBOARD_KEYCODE_F3,             /* F3 */
    [70] = KEYBOARD_KEYCODE_F4,             /* F4 */
    [71] = KEYBOARD_KEYCODE_F5,             /* F5 */
    [72] = KEYBOARD_KEYCODE_F6,             /* F6 */
    [73] = KEYBOARD_KEYCODE_F7,             /* F7 */
    [74] = KEYBOARD_KEYCODE_F8,             /* F8 */
    [75] = KEYBOARD_KEYCODE_F9,             /* F9 */
    [76] = KEYBOARD_KEYCODE_F10,            /* F10 */
    [77] = KEYBOARD_KEYCODE_NUMLOCK,        /* Num Lock */
    [78] = KEYBOARD_KEYCODE_SCROLLLOCK,     /* Scroll Lock */
    [79] = KEYBOARD_KEYCODE_NONE,           /* Keypad 7 */
    [80] = KEYBOARD_KEYCODE_NONE,           /* Keypad 8 */
    [81] = KEYBOARD_KEYCODE_NONE,           /* Keypad 9 */
    [82] = KEYBOARD_KEYCODE_NONE,           /* Keypad - */
    [83] = KEYBOARD_KEYCODE_NONE,           /* Keypad 4 */
    [84] = KEYBOARD_KEYCODE_NONE,           /* Keypad 5 */
    [85] = KEYBOARD_KEYCODE_NONE,           /* Keypad 6 */
    [86] = KEYBOARD_KEYCODE_NONE,           /* Keypad + */
    [87] = KEYBOARD_KEYCODE_NONE,           /* Keypad 1 */
    [88] = KEYBOARD_KEYCODE_NONE,           /* Keypad 2 */
    [89] = KEYBOARD_KEYCODE_NONE,           /* Keypad 3 */
    [90] = KEYBOARD_KEYCODE_NONE,           /* Keypad 0 */
    [91] = KEYBOARD_KEYCODE_NONE,           /* Keypad . */
    [92] = KEYBOARD_KEYCODE_RIGHT_ALT,      /* AltGr */
    [93] = KEYBOARD_KEYCODE_LEFT_SUPER,     /* Left Super */
    [94] = KEYBOARD_KEYCODE_RIGHT_SUPER,    /* Right Super */
    [95] = KEYBOARD_KEYCODE_F11,            /* Menu <? f11 ?> */
    [96] = KEYBOARD_KEYCODE_F12,            /* Cancel <? f12 ?> */
    [97] = KEYBOARD_KEYCODE_NONE,           /* Redo */
    [98] = KEYBOARD_KEYCODE_NONE,           /* Undo */
    [99] = KEYBOARD_KEYCODE_NONE,           /* Find */
    [100] = KEYBOARD_KEYCODE_NONE,          /* Help */
    [101] = KEYBOARD_KEYCODE_HOME,          /* Home */
    [102] = KEYBOARD_KEYCODE_LEFT,          /* Left Arrow */
    [103] = KEYBOARD_KEYCODE_UP,            /* Up Arrow */
    [104] = KEYBOARD_KEYCODE_RIGHT,         /* Right Arrow */
    [105] = KEYBOARD_KEYCODE_RIGHT_CONTROL, /* Down Arrow <? right control ?> */
    [106] = KEYBOARD_KEYCODE_PAGEUP,        /* Page Up */
    [107] = KEYBOARD_KEYCODE_PAGEDOWN,      /* Page Down */
    [108] = KEYBOARD_KEYCODE_RIGHT_ALT,     /* End <? right alt ?> */
    [109] = KEYBOARD_KEYCODE_NONE,          /* Begin */
    [110] = KEYBOARD_KEYCODE_HOME,          /* Select <? home ?> */
    [111] = KEYBOARD_KEYCODE_UP,            /* Print <? up ?> */
    [112] = KEYBOARD_KEYCODE_PAGEUP,        /* Execute <? page up ?> */
    [113] = KEYBOARD_KEYCODE_LEFT,          /* Insert <? left ?> */
    [114] = KEYBOARD_KEYCODE_RIGHT,         /* Delete <? right ?> */
    [115] = KEYBOARD_KEYCODE_END,           /* Pause  <? end ?> */
    [116] = KEYBOARD_KEYCODE_DOWN,          /* F11 <? down ?>*/
    [117] = KEYBOARD_KEYCODE_PAGEDOWN,      /* F12 <? page down ?> */
    [118] = KEYBOARD_KEYCODE_NONE,          /* F13 */
    [119] = KEYBOARD_KEYCODE_DELETE,        /* F14 <? delete ?> */
    [120] = KEYBOARD_KEYCODE_NONE,          /* F15 */
    [121] = KEYBOARD_KEYCODE_NONE,          /* F16 */
    [122] = KEYBOARD_KEYCODE_NONE,          /* F17 */
    [123] = KEYBOARD_KEYCODE_NONE,          /* F18 */
    [124] = KEYBOARD_KEYCODE_NONE,          /* F19 */
    [125] = KEYBOARD_KEYCODE_NONE,          /* F20 */
    [126] = KEYBOARD_KEYCODE_NONE,          /* F21 */
    [127] = KEYBOARD_KEYCODE_NONE,          /* F22 */
    [128] = KEYBOARD_KEYCODE_NONE,          /* F23 */
    [129] = KEYBOARD_KEYCODE_NONE,          /* F24 */
    [130] = KEYBOARD_KEYCODE_NONE,          /* Keypad Enter */
    [131] = KEYBOARD_KEYCODE_RIGHT_CONTROL, /* Right Control */
    [132] = KEYBOARD_KEYCODE_NONE,          /* Keypad / */
    [133] = KEYBOARD_KEYCODE_RIGHT_SUPER,   /* Keypad = <? right super ?> */
    [134] = KEYBOARD_KEYCODE_LEFT_META,     /* Left Meta */
    [135] = KEYBOARD_KEYCODE_RIGHT_META,    /* Right Meta */
    [136] = KEYBOARD_KEYCODE_RIGHT_ALT,     /* Right Alt */
    [137] = KEYBOARD_KEYCODE_LEFT_SUPER,    /* Left Super */
    [138] = KEYBOARD_KEYCODE_RIGHT_SUPER,   /* Right Super */
    [139] = KEYBOARD_KEYCODE_NONE,          /* Left Hyper */
    [140] = KEYBOARD_KEYCODE_NONE,          /* Right Hyper */
    [141] = KEYBOARD_KEYCODE_RIGHT_ALT,     /* AltGr */
    [142] = KEYBOARD_KEYCODE_NONE,          /* Level5 Shift */
    [143] = KEYBOARD_KEYCODE_NONE,          /* Level3 Latch */
    [144] = KEYBOARD_KEYCODE_NONE,          /* Level3 Lock */
    [145] = KEYBOARD_KEYCODE_NONE,          /* Level5 Latch */
    [146] = KEYBOARD_KEYCODE_NONE,          /* Level5 Lock */
    [147] = KEYBOARD_KEYCODE_NONE,          /* Group Shift */
    [148] = KEYBOARD_KEYCODE_NONE,          /* Group Latch */
    [149] = KEYBOARD_KEYCODE_NONE,          /* Group Lock */
    [150] = KEYBOARD_KEYCODE_NONE,          /* Next Group */
    [151] = KEYBOARD_KEYCODE_NONE,          /* Next Group Lock */
    [152] = KEYBOARD_KEYCODE_NONE,          /* Previous Group */
    [153] = KEYBOARD_KEYCODE_NONE,          /* Previous Group Lock */
    [154] = KEYBOARD_KEYCODE_NONE,          /* First Group */
    [155] = KEYBOARD_KEYCODE_NONE,          /* First Group Lock */
    [156] = KEYBOARD_KEYCODE_NONE,          /* Last Group */
    [157] = KEYBOARD_KEYCODE_NONE,          /* Last Group Lock */
    [158] = KEYBOARD_KEYCODE_RIGHT_ALT,     /* AltGr */
    [159] = KEYBOARD_KEYCODE_NONE,          /* Level3 Latch */
    [160] = KEYBOARD_KEYCODE_NONE,          /* Level3 Lock */
    [161] = KEYBOARD_KEYCODE_NONE,          /* Level5 Shift */
    [162] = KEYBOARD_KEYCODE_NONE,          /* Level5 Latch */
    [163] = KEYBOARD_KEYCODE_NONE,          /* Level5 Lock */
    [164] = KEYBOARD_KEYCODE_NONE,          /* Group Shift */
    [165] = KEYBOARD_KEYCODE_NONE,          /* Group Latch */
    [166] = KEYBOARD_KEYCODE_NONE,          /* Group Lock */
    [167] = KEYBOARD_KEYCODE_NONE,          /* ISO_Next_Group */
    [168] = KEYBOARD_KEYCODE_NONE,          /* ISO_Next_Group_Lock */
    [169] = KEYBOARD_KEYCODE_NONE,          /* ISO_Prev_Group */
    [170] = KEYBOARD_KEYCODE_NONE,          /* ISO_Prev_Group_Lock */
    [171] = KEYBOARD_KEYCODE_NONE,          /* ISO_First_Group */
    [172] = KEYBOARD_KEYCODE_NONE,          /* ISO_First_Group_Lock */
    [173] = KEYBOARD_KEYCODE_NONE,          /* ISO_Last_Group */
    [174] = KEYBOARD_KEYCODE_NONE,          /* ISO_Last_Group_Lock */
    [175] = KEYBOARD_KEYCODE_NONE,          /* KP_Decimal */
    [176] = KEYBOARD_KEYCODE_NONE,          /* KP_Separator */
    [177] = KEYBOARD_KEYCODE_NONE,          /* KP_Equal */
    [178] = KEYBOARD_KEYCODE_NONE,          /* KP_Multiply */
    [179] = KEYBOARD_KEYCODE_NONE,          /* KP_Add */
    [180] = KEYBOARD_KEYCODE_NONE,          /* KP_Subtract */
    [181] = KEYBOARD_KEYCODE_NONE,          /* KP_Divide */
    [182] = KEYBOARD_KEYCODE_NONE,          /* KP_Enter */
    [183] = KEYBOARD_KEYCODE_NONE,          /* KP_Home */
    [184] = KEYBOARD_KEYCODE_NONE,          /* KP_Up */
    [185] = KEYBOARD_KEYCODE_NONE,          /* KP_Prior */
    [186] = KEYBOARD_KEYCODE_NONE,          /* KP_Left */
    [187] = KEYBOARD_KEYCODE_NONE,          /* KP_Begin */
    [188] = KEYBOARD_KEYCODE_NONE,          /* KP_Right */
    [189] = KEYBOARD_KEYCODE_NONE,          /* KP_End */
    [190] = KEYBOARD_KEYCODE_NONE,          /* KP_Down */
    [191] = KEYBOARD_KEYCODE_NONE,          /* KP_Next */
    [192] = KEYBOARD_KEYCODE_NONE,          /* KP_Insert */
    [193] = KEYBOARD_KEYCODE_NONE,          /* KP_Delete */
    [194] = KEYBOARD_KEYCODE_NONE,          /* KP_0 */
    [195] = KEYBOARD_KEYCODE_NONE,          /* KP_1 */
    [196] = KEYBOARD_KEYCODE_NONE,          /* KP_2 */
    [197] = KEYBOARD_KEYCODE_NONE,          /* KP_3 */
    [198] = KEYBOARD_KEYCODE_NONE,          /* KP_4 */
    [199] = KEYBOARD_KEYCODE_NONE,          /* KP_5 */
    [200] = KEYBOARD_KEYCODE_NONE,          /* KP_6 */
    [201] = KEYBOARD_KEYCODE_NONE,          /* KP_7 */
    [202] = KEYBOARD_KEYCODE_NONE,          /* KP_8 */
    [203] = KEYBOARD_KEYCODE_NONE,          /* KP_9 */
    [204] = KEYBOARD_KEYCODE_NONE,          /* F25 */
    [205] = KEYBOARD_KEYCODE_NONE,          /* F26 */
    [206] = KEYBOARD_KEYCODE_NONE,          /* F27 */
    [207] = KEYBOARD_KEYCODE_NONE,          /* F28 */
    [208] = KEYBOARD_KEYCODE_NONE,          /* F29 */
    [209] = KEYBOARD_KEYCODE_NONE,          /* F30 */
    [210] = KEYBOARD_KEYCODE_NONE,          /* F31 */
    [211] = KEYBOARD_KEYCODE_NONE,          /* F32 */
    [212] = KEYBOARD_KEYCODE_NONE,          /* F33 */
    [213] = KEYBOARD_KEYCODE_NONE,          /* F34 */
    [214] = KEYBOARD_KEYCODE_NONE,          /* F35 */
    [215] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [216] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [217] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [218] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [219] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [220] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [221] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [222] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [223] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [224] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [225] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [226] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [227] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [228] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [229] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [230] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [231] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [232] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [233] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [234] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [235] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [236] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [237] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [238] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [239] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [240] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [241] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [242] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [243] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [244] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [245] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [246] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [247] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [248] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [249] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [250] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [251] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [252] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [253] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [254] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
    [255] = KEYBOARD_KEYCODE_NONE,          /* NoSymbol */
};

static keyboard_keycode_state_t __platform_input_keyboard_keycode_states[KEYBOARD_KEYCODE_COUNT] = {
    KEYBOARD_KEYCODE_STATE_NONE,
};

keyboard_keycode_t __platform_input_keyboard_keycode(xcb_keycode_t keycode) {
  return platform_input_keyboard_keycode_mapping[keycode];
}

keyboard_keycode_t __platform_input_keyboard_keysymbol(xcb_keysym_t keysymbol) {
  switch (keysymbol) {
    /* letters */
    case XK_a:
    case XK_A:
      return KEYBOARD_KEYCODE_A;
    case XK_b:
    case XK_B:
      return KEYBOARD_KEYCODE_B;
    case XK_c:
    case XK_C:
      return KEYBOARD_KEYCODE_C;
    case XK_d:
    case XK_D:
      return KEYBOARD_KEYCODE_D;
    case XK_e:
    case XK_E:
      return KEYBOARD_KEYCODE_E;
    case XK_f:
    case XK_F:
      return KEYBOARD_KEYCODE_F;
    case XK_g:
    case XK_G:
      return KEYBOARD_KEYCODE_G;
    case XK_h:
    case XK_H:
      return KEYBOARD_KEYCODE_H;
    case XK_i:
    case XK_I:
      return KEYBOARD_KEYCODE_I;
    case XK_j:
    case XK_J:
      return KEYBOARD_KEYCODE_J;
    case XK_k:
    case XK_K:
      return KEYBOARD_KEYCODE_K;
    case XK_l:
    case XK_L:
      return KEYBOARD_KEYCODE_L;
    case XK_m:
    case XK_M:
      return KEYBOARD_KEYCODE_M;
    case XK_n:
    case XK_N:
      return KEYBOARD_KEYCODE_N;
    case XK_o:
    case XK_O:
      return KEYBOARD_KEYCODE_O;
    case XK_p:
    case XK_P:
      return KEYBOARD_KEYCODE_P;
    case XK_q:
    case XK_Q:
      return KEYBOARD_KEYCODE_Q;
    case XK_r:
    case XK_R:
      return KEYBOARD_KEYCODE_R;
    case XK_s:
    case XK_S:
      return KEYBOARD_KEYCODE_S;
    case XK_t:
    case XK_T:
      return KEYBOARD_KEYCODE_T;
    case XK_u:
    case XK_U:
      return KEYBOARD_KEYCODE_U;
    case XK_v:
    case XK_V:
      return KEYBOARD_KEYCODE_V;
    case XK_w:
    case XK_W:
      return KEYBOARD_KEYCODE_W;
    case XK_x:
    case XK_X:
      return KEYBOARD_KEYCODE_X;
    case XK_y:
    case XK_Y:
      return KEYBOARD_KEYCODE_Y;
    case XK_z:
    case XK_Z:
      return KEYBOARD_KEYCODE_Z;

    /* numbers */
    case XK_0:
      return KEYBOARD_KEYCODE_0;
    case XK_1:
      return KEYBOARD_KEYCODE_1;
    case XK_2:
      return KEYBOARD_KEYCODE_2;
    case XK_3:
      return KEYBOARD_KEYCODE_3;
    case XK_4:
      return KEYBOARD_KEYCODE_4;
    case XK_5:
      return KEYBOARD_KEYCODE_5;
    case XK_6:
      return KEYBOARD_KEYCODE_6;
    case XK_7:
      return KEYBOARD_KEYCODE_7;
    case XK_8:
      return KEYBOARD_KEYCODE_8;
    case XK_9:
      return KEYBOARD_KEYCODE_9;

    /* general */
    case XK_Tab:
      return KEYBOARD_KEYCODE_TAB;
    case XK_Return:
      return KEYBOARD_KEYCODE_ENTER;
    case XK_space:
      return KEYBOARD_KEYCODE_SPACE;
    case XK_Escape:
      return KEYBOARD_KEYCODE_ESCAPE;
    case XK_BackSpace:
      return KEYBOARD_KEYCODE_BACKSPACE;

    /* arrows */
    case XK_Up:
      return KEYBOARD_KEYCODE_UP;
    case XK_Down:
      return KEYBOARD_KEYCODE_DOWN;
    case XK_Left:
      return KEYBOARD_KEYCODE_LEFT;
    case XK_Right:
      return KEYBOARD_KEYCODE_RIGHT;

    /* modifiers */
    case XK_Alt_L:
      return KEYBOARD_KEYCODE_LEFT_ALT;
    case XK_Meta_L:
      return KEYBOARD_KEYCODE_LEFT_META;
    case XK_Alt_R:
      return KEYBOARD_KEYCODE_RIGHT_ALT;
    case XK_Shift_L:
      return KEYBOARD_KEYCODE_LEFT_SHIFT;
    case XK_Super_L:
      return KEYBOARD_KEYCODE_LEFT_SUPER;
    case XK_Meta_R:
      return KEYBOARD_KEYCODE_RIGHT_META;
    case XK_Shift_R:
      return KEYBOARD_KEYCODE_RIGHT_SHIFT;
    case XK_Super_R:
      return KEYBOARD_KEYCODE_RIGHT_SUPER;
    case XK_bracketleft:
      return KEYBOARD_KEYCODE_LEFT_BRACKET;
    case XK_Control_L:
      return KEYBOARD_KEYCODE_LEFT_CONTROL;
    case XK_bracketright:
      return KEYBOARD_KEYCODE_RIGHT_BRACKET;
    case XK_Control_R:
      return KEYBOARD_KEYCODE_RIGHT_CONTROL;

    /* functions */
    case XK_F1:
      return KEYBOARD_KEYCODE_F1;
    case XK_F2:
      return KEYBOARD_KEYCODE_F2;
    case XK_F3:
      return KEYBOARD_KEYCODE_F3;
    case XK_F4:
      return KEYBOARD_KEYCODE_F4;
    case XK_F5:
      return KEYBOARD_KEYCODE_F5;
    case XK_F6:
      return KEYBOARD_KEYCODE_F6;
    case XK_F7:
      return KEYBOARD_KEYCODE_F7;
    case XK_F8:
      return KEYBOARD_KEYCODE_F8;
    case XK_F9:
      return KEYBOARD_KEYCODE_F9;
    case XK_F10:
      return KEYBOARD_KEYCODE_F10;
    case XK_F11:
      return KEYBOARD_KEYCODE_F11;
    case XK_F12:
      return KEYBOARD_KEYCODE_F12;

    /* general */
    case XK_End:
      return KEYBOARD_KEYCODE_END;
    case XK_Home:
      return KEYBOARD_KEYCODE_HOME;
    case XK_Delete:
      return KEYBOARD_KEYCODE_DELETE;
    case XK_Insert:
      return KEYBOARD_KEYCODE_INSERT;
    case XK_Page_Up:
      return KEYBOARD_KEYCODE_PAGEUP;
    case XK_Page_Down:
      return KEYBOARD_KEYCODE_PAGEDOWN;

    /* general */
    case XK_Num_Lock:
      return KEYBOARD_KEYCODE_NUMLOCK;
    case XK_Caps_Lock:
      return KEYBOARD_KEYCODE_CAPSLOCK;
    case XK_Scroll_Lock:
      return KEYBOARD_KEYCODE_SCROLLLOCK;

    /* general */
    case XK_Pause:
      return KEYBOARD_KEYCODE_PAUSE;
    case XK_Print:
      return KEYBOARD_KEYCODE_PRINTSCREEN;

    /* symbols */
    case XK_comma:
      return KEYBOARD_KEYCODE_COMMA;
    case XK_grave:
      return KEYBOARD_KEYCODE_GRAVE;
    case XK_minus:
      return KEYBOARD_KEYCODE_MINUS;
    case XK_slash:
      return KEYBOARD_KEYCODE_SLASH;
    case XK_equal:
      return KEYBOARD_KEYCODE_EQUALS;
    case XK_period:
      return KEYBOARD_KEYCODE_PERIOD;
    case XK_backslash:
      return KEYBOARD_KEYCODE_BACKSLASH;
    case XK_semicolon:
      return KEYBOARD_KEYCODE_SEMICOLON;
    case XK_apostrophe:
      return KEYBOARD_KEYCODE_APOSTROPHE;

    default:
      return KEYBOARD_KEYCODE_NONE;
  }
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
