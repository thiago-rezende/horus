/* horus input layer [ linux ] */
#include <horus/platform/linux/input/keyboard.h>

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
    [66] = KEYBOARD_KEYCODE_CAPS_LOCK,      /* Caps Lock */
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
    [77] = KEYBOARD_KEYCODE_NUM_LOCK,       /* Num Lock */
    [78] = KEYBOARD_KEYCODE_SCROLL_LOCK,    /* Scroll Lock */
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
    [106] = KEYBOARD_KEYCODE_PAGE_UP,       /* Page Up */
    [107] = KEYBOARD_KEYCODE_PAGE_DOWN,     /* Page Down */
    [108] = KEYBOARD_KEYCODE_RIGHT_ALT,     /* End <? right alt ?> */
    [109] = KEYBOARD_KEYCODE_NONE,          /* Begin */
    [110] = KEYBOARD_KEYCODE_HOME,          /* Select <? home ?> */
    [111] = KEYBOARD_KEYCODE_UP,            /* Print <? up ?> */
    [112] = KEYBOARD_KEYCODE_PAGE_UP,       /* Execute <? page up ?> */
    [113] = KEYBOARD_KEYCODE_LEFT,          /* Insert <? left ?> */
    [114] = KEYBOARD_KEYCODE_RIGHT,         /* Delete <? right ?> */
    [115] = KEYBOARD_KEYCODE_END,           /* Pause  <? end ?> */
    [116] = KEYBOARD_KEYCODE_DOWN,          /* F11 <? down ?>*/
    [117] = KEYBOARD_KEYCODE_PAGE_DOWN,     /* F12 <? page down ?> */
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
    [KEYBOARD_KEYCODE_NONE] = KEYBOARD_KEYCODE_STATE_NONE,

    /* letters */
    [KEYBOARD_KEYCODE_A] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_B] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_C] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_D] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_E] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_G] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_H] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_I] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_J] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_K] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_L] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_M] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_N] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_O] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_P] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_Q] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_R] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_S] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_T] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_U] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_V] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_W] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_X] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_Y] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_Z] = KEYBOARD_KEYCODE_STATE_NONE,

    /* numbers */
    [KEYBOARD_KEYCODE_0] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_1] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_2] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_3] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_4] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_5] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_6] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_7] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_8] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_9] = KEYBOARD_KEYCODE_STATE_NONE,

    /* general */
    [KEYBOARD_KEYCODE_TAB] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_ENTER] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_SPACE] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_ESCAPE] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_BACKSPACE] = KEYBOARD_KEYCODE_STATE_NONE,

    /* arrows */
    [KEYBOARD_KEYCODE_UP] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_DOWN] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT] = KEYBOARD_KEYCODE_STATE_NONE,

    /* modifiers */
    [KEYBOARD_KEYCODE_PRINT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_PAGE_UP] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT_ALT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_NUM_LOCK] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_CAPS_LOCK] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT_META] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_PAGE_DOWN] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT_ALT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT_SHIFT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT_SUPER] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT_META] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT_SHIFT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT_SUPER] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_SCROLL_LOCK] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT_BRACKET] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_LEFT_CONTROL] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT_BRACKET] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_RIGHT_CONTROL] = KEYBOARD_KEYCODE_STATE_NONE,

    /* functions */
    [KEYBOARD_KEYCODE_F1] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F2] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F3] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F4] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F5] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F6] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F7] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F8] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F9] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F10] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F11] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_F12] = KEYBOARD_KEYCODE_STATE_NONE,

    /* general */
    [KEYBOARD_KEYCODE_END] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_HOME] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_DELETE] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_INSERT] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_PAGEUP] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_PAGEDOWN] = KEYBOARD_KEYCODE_STATE_NONE,

    /* general */
    [KEYBOARD_KEYCODE_NUMLOCK] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_CAPSLOCK] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_SCROLLLOCK] = KEYBOARD_KEYCODE_STATE_NONE,

    /* general */
    [KEYBOARD_KEYCODE_PAUSE] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_PRINTSCREEN] = KEYBOARD_KEYCODE_STATE_NONE,

    /* symbols */
    [KEYBOARD_KEYCODE_COMMA] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_GRAVE] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_MINUS] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_SLASH] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_EQUALS] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_PERIOD] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_BACKSLASH] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_SEMICOLON] = KEYBOARD_KEYCODE_STATE_NONE,
    [KEYBOARD_KEYCODE_APOSTROPHE] = KEYBOARD_KEYCODE_STATE_NONE,
};

keyboard_keycode_t __platform_input_keyboard_keycode(xcb_keycode_t keycode) {
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
