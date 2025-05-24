/* horus input layer [ linux ] */
#include <horus/platform/linux/input/mouse.h>

#define PLATFORM_INPUT_MOUSE_BUTTON_COUNT 256

static const mouse_button_t platform_input_mouse_button_mapping[PLATFORM_INPUT_MOUSE_BUTTON_COUNT] = {
    [0] = MOUSE_BUTTON_NONE,          /* */
    [1] = MOUSE_BUTTON_LEFT,          /* */
    [2] = MOUSE_BUTTON_MIDDLE,        /* */
    [3] = MOUSE_BUTTON_RIGHT,         /* */
    [4] = MOUSE_BUTTON_NONE,          /* */
    [5] = MOUSE_BUTTON_NONE,          /* */
    [6] = MOUSE_BUTTON_NONE,          /* */
    [7] = MOUSE_BUTTON_NONE,          /* */
    [8] = MOUSE_BUTTON_SIDE_BACKWARD, /* */
    [9] = MOUSE_BUTTON_SIDE_FORWARD,  /* */
    [10] = MOUSE_BUTTON_NONE,         /* */
    [11] = MOUSE_BUTTON_NONE,         /* */
    [12] = MOUSE_BUTTON_NONE,         /* */
    [13] = MOUSE_BUTTON_NONE,         /* */
    [14] = MOUSE_BUTTON_NONE,         /* */
    [15] = MOUSE_BUTTON_NONE,         /* */
    [16] = MOUSE_BUTTON_NONE,         /* */
    [17] = MOUSE_BUTTON_NONE,         /* */
    [18] = MOUSE_BUTTON_NONE,         /* */
    [19] = MOUSE_BUTTON_NONE,         /* */
    [20] = MOUSE_BUTTON_NONE,         /* */
    [21] = MOUSE_BUTTON_NONE,         /* */
    [22] = MOUSE_BUTTON_NONE,         /* */
    [23] = MOUSE_BUTTON_NONE,         /* */
    [24] = MOUSE_BUTTON_NONE,         /* */
    [25] = MOUSE_BUTTON_NONE,         /* */
    [26] = MOUSE_BUTTON_NONE,         /* */
    [27] = MOUSE_BUTTON_NONE,         /* */
    [28] = MOUSE_BUTTON_NONE,         /* */
    [29] = MOUSE_BUTTON_NONE,         /* */
    [30] = MOUSE_BUTTON_NONE,         /* */
    [31] = MOUSE_BUTTON_NONE,         /* */
    [32] = MOUSE_BUTTON_NONE,         /* */
    [33] = MOUSE_BUTTON_NONE,         /* */
    [34] = MOUSE_BUTTON_NONE,         /* */
    [35] = MOUSE_BUTTON_NONE,         /* */
    [36] = MOUSE_BUTTON_NONE,         /* */
    [37] = MOUSE_BUTTON_NONE,         /* */
    [38] = MOUSE_BUTTON_NONE,         /* */
    [39] = MOUSE_BUTTON_NONE,         /* */
    [40] = MOUSE_BUTTON_NONE,         /* */
    [41] = MOUSE_BUTTON_NONE,         /* */
    [42] = MOUSE_BUTTON_NONE,         /* */
    [43] = MOUSE_BUTTON_NONE,         /* */
    [44] = MOUSE_BUTTON_NONE,         /* */
    [45] = MOUSE_BUTTON_NONE,         /* */
    [46] = MOUSE_BUTTON_NONE,         /* */
    [47] = MOUSE_BUTTON_NONE,         /* */
    [48] = MOUSE_BUTTON_NONE,         /* */
    [49] = MOUSE_BUTTON_NONE,         /* */
    [50] = MOUSE_BUTTON_NONE,         /* */
    [51] = MOUSE_BUTTON_NONE,         /* */
    [52] = MOUSE_BUTTON_NONE,         /* */
    [53] = MOUSE_BUTTON_NONE,         /* */
    [54] = MOUSE_BUTTON_NONE,         /* */
    [55] = MOUSE_BUTTON_NONE,         /* */
    [56] = MOUSE_BUTTON_NONE,         /* */
    [57] = MOUSE_BUTTON_NONE,         /* */
    [58] = MOUSE_BUTTON_NONE,         /* */
    [59] = MOUSE_BUTTON_NONE,         /* */
    [60] = MOUSE_BUTTON_NONE,         /* */
    [61] = MOUSE_BUTTON_NONE,         /* */
    [62] = MOUSE_BUTTON_NONE,         /* */
    [63] = MOUSE_BUTTON_NONE,         /* */
    [64] = MOUSE_BUTTON_NONE,         /* */
    [65] = MOUSE_BUTTON_NONE,         /* */
    [66] = MOUSE_BUTTON_NONE,         /* */
    [67] = MOUSE_BUTTON_NONE,         /* */
    [68] = MOUSE_BUTTON_NONE,         /* */
    [69] = MOUSE_BUTTON_NONE,         /* */
    [70] = MOUSE_BUTTON_NONE,         /* */
    [71] = MOUSE_BUTTON_NONE,         /* */
    [72] = MOUSE_BUTTON_NONE,         /* */
    [73] = MOUSE_BUTTON_NONE,         /* */
    [74] = MOUSE_BUTTON_NONE,         /* */
    [75] = MOUSE_BUTTON_NONE,         /* */
    [76] = MOUSE_BUTTON_NONE,         /* */
    [77] = MOUSE_BUTTON_NONE,         /* */
    [78] = MOUSE_BUTTON_NONE,         /* */
    [79] = MOUSE_BUTTON_NONE,         /* */
    [80] = MOUSE_BUTTON_NONE,         /* */
    [81] = MOUSE_BUTTON_NONE,         /* */
    [82] = MOUSE_BUTTON_NONE,         /* */
    [83] = MOUSE_BUTTON_NONE,         /* */
    [84] = MOUSE_BUTTON_NONE,         /* */
    [85] = MOUSE_BUTTON_NONE,         /* */
    [86] = MOUSE_BUTTON_NONE,         /* */
    [87] = MOUSE_BUTTON_NONE,         /* */
    [88] = MOUSE_BUTTON_NONE,         /* */
    [89] = MOUSE_BUTTON_NONE,         /* */
    [90] = MOUSE_BUTTON_NONE,         /* */
    [91] = MOUSE_BUTTON_NONE,         /* */
    [92] = MOUSE_BUTTON_NONE,         /* */
    [93] = MOUSE_BUTTON_NONE,         /* */
    [94] = MOUSE_BUTTON_NONE,         /* */
    [95] = MOUSE_BUTTON_NONE,         /* */
    [96] = MOUSE_BUTTON_NONE,         /* */
    [97] = MOUSE_BUTTON_NONE,         /* */
    [98] = MOUSE_BUTTON_NONE,         /* */
    [99] = MOUSE_BUTTON_NONE,         /* */
    [100] = MOUSE_BUTTON_NONE,        /* */
    [101] = MOUSE_BUTTON_NONE,        /* */
    [102] = MOUSE_BUTTON_NONE,        /* */
    [103] = MOUSE_BUTTON_NONE,        /* */
    [104] = MOUSE_BUTTON_NONE,        /* */
    [105] = MOUSE_BUTTON_NONE,        /* */
    [106] = MOUSE_BUTTON_NONE,        /* */
    [107] = MOUSE_BUTTON_NONE,        /* */
    [108] = MOUSE_BUTTON_NONE,        /* */
    [109] = MOUSE_BUTTON_NONE,        /* */
    [110] = MOUSE_BUTTON_NONE,        /* */
    [111] = MOUSE_BUTTON_NONE,        /* */
    [112] = MOUSE_BUTTON_NONE,        /* */
    [113] = MOUSE_BUTTON_NONE,        /* */
    [114] = MOUSE_BUTTON_NONE,        /* */
    [115] = MOUSE_BUTTON_NONE,        /* */
    [116] = MOUSE_BUTTON_NONE,        /* */
    [117] = MOUSE_BUTTON_NONE,        /* */
    [118] = MOUSE_BUTTON_NONE,        /* */
    [119] = MOUSE_BUTTON_NONE,        /* */
    [120] = MOUSE_BUTTON_NONE,        /* */
    [121] = MOUSE_BUTTON_NONE,        /* */
    [122] = MOUSE_BUTTON_NONE,        /* */
    [123] = MOUSE_BUTTON_NONE,        /* */
    [124] = MOUSE_BUTTON_NONE,        /* */
    [125] = MOUSE_BUTTON_NONE,        /* */
    [126] = MOUSE_BUTTON_NONE,        /* */
    [127] = MOUSE_BUTTON_NONE,        /* */
    [128] = MOUSE_BUTTON_NONE,        /* */
    [129] = MOUSE_BUTTON_NONE,        /* */
    [130] = MOUSE_BUTTON_NONE,        /* */
    [131] = MOUSE_BUTTON_NONE,        /* */
    [132] = MOUSE_BUTTON_NONE,        /* */
    [133] = MOUSE_BUTTON_NONE,        /* */
    [134] = MOUSE_BUTTON_NONE,        /* */
    [135] = MOUSE_BUTTON_NONE,        /* */
    [136] = MOUSE_BUTTON_NONE,        /* */
    [137] = MOUSE_BUTTON_NONE,        /* */
    [138] = MOUSE_BUTTON_NONE,        /* */
    [139] = MOUSE_BUTTON_NONE,        /* */
    [140] = MOUSE_BUTTON_NONE,        /* */
    [141] = MOUSE_BUTTON_NONE,        /* */
    [142] = MOUSE_BUTTON_NONE,        /* */
    [143] = MOUSE_BUTTON_NONE,        /* */
    [144] = MOUSE_BUTTON_NONE,        /* */
    [145] = MOUSE_BUTTON_NONE,        /* */
    [146] = MOUSE_BUTTON_NONE,        /* */
    [147] = MOUSE_BUTTON_NONE,        /* */
    [148] = MOUSE_BUTTON_NONE,        /* */
    [149] = MOUSE_BUTTON_NONE,        /* */
    [150] = MOUSE_BUTTON_NONE,        /* */
    [151] = MOUSE_BUTTON_NONE,        /* */
    [152] = MOUSE_BUTTON_NONE,        /* */
    [153] = MOUSE_BUTTON_NONE,        /* */
    [154] = MOUSE_BUTTON_NONE,        /* */
    [155] = MOUSE_BUTTON_NONE,        /* */
    [156] = MOUSE_BUTTON_NONE,        /* */
    [157] = MOUSE_BUTTON_NONE,        /* */
    [158] = MOUSE_BUTTON_NONE,        /* */
    [159] = MOUSE_BUTTON_NONE,        /* */
    [160] = MOUSE_BUTTON_NONE,        /* */
    [161] = MOUSE_BUTTON_NONE,        /* */
    [162] = MOUSE_BUTTON_NONE,        /* */
    [163] = MOUSE_BUTTON_NONE,        /* */
    [164] = MOUSE_BUTTON_NONE,        /* */
    [165] = MOUSE_BUTTON_NONE,        /* */
    [166] = MOUSE_BUTTON_NONE,        /* */
    [167] = MOUSE_BUTTON_NONE,        /* */
    [168] = MOUSE_BUTTON_NONE,        /* */
    [169] = MOUSE_BUTTON_NONE,        /* */
    [170] = MOUSE_BUTTON_NONE,        /* */
    [171] = MOUSE_BUTTON_NONE,        /* */
    [172] = MOUSE_BUTTON_NONE,        /* */
    [173] = MOUSE_BUTTON_NONE,        /* */
    [174] = MOUSE_BUTTON_NONE,        /* */
    [175] = MOUSE_BUTTON_NONE,        /* */
    [176] = MOUSE_BUTTON_NONE,        /* */
    [177] = MOUSE_BUTTON_NONE,        /* */
    [178] = MOUSE_BUTTON_NONE,        /* */
    [179] = MOUSE_BUTTON_NONE,        /* */
    [180] = MOUSE_BUTTON_NONE,        /* */
    [181] = MOUSE_BUTTON_NONE,        /* */
    [182] = MOUSE_BUTTON_NONE,        /* */
    [183] = MOUSE_BUTTON_NONE,        /* */
    [184] = MOUSE_BUTTON_NONE,        /* */
    [185] = MOUSE_BUTTON_NONE,        /* */
    [186] = MOUSE_BUTTON_NONE,        /* */
    [187] = MOUSE_BUTTON_NONE,        /* */
    [188] = MOUSE_BUTTON_NONE,        /* */
    [189] = MOUSE_BUTTON_NONE,        /* */
    [190] = MOUSE_BUTTON_NONE,        /* */
    [191] = MOUSE_BUTTON_NONE,        /* */
    [192] = MOUSE_BUTTON_NONE,        /* */
    [193] = MOUSE_BUTTON_NONE,        /* */
    [194] = MOUSE_BUTTON_NONE,        /* */
    [195] = MOUSE_BUTTON_NONE,        /* */
    [196] = MOUSE_BUTTON_NONE,        /* */
    [197] = MOUSE_BUTTON_NONE,        /* */
    [198] = MOUSE_BUTTON_NONE,        /* */
    [199] = MOUSE_BUTTON_NONE,        /* */
    [200] = MOUSE_BUTTON_NONE,        /* */
    [201] = MOUSE_BUTTON_NONE,        /* */
    [202] = MOUSE_BUTTON_NONE,        /* */
    [203] = MOUSE_BUTTON_NONE,        /* */
    [204] = MOUSE_BUTTON_NONE,        /* */
    [205] = MOUSE_BUTTON_NONE,        /* */
    [206] = MOUSE_BUTTON_NONE,        /* */
    [207] = MOUSE_BUTTON_NONE,        /* */
    [208] = MOUSE_BUTTON_NONE,        /* */
    [209] = MOUSE_BUTTON_NONE,        /* */
    [210] = MOUSE_BUTTON_NONE,        /* */
    [211] = MOUSE_BUTTON_NONE,        /* */
    [212] = MOUSE_BUTTON_NONE,        /* */
    [213] = MOUSE_BUTTON_NONE,        /* */
    [214] = MOUSE_BUTTON_NONE,        /* */
    [215] = MOUSE_BUTTON_NONE,        /* */
    [216] = MOUSE_BUTTON_NONE,        /* */
    [217] = MOUSE_BUTTON_NONE,        /* */
    [218] = MOUSE_BUTTON_NONE,        /* */
    [219] = MOUSE_BUTTON_NONE,        /* */
    [220] = MOUSE_BUTTON_NONE,        /* */
    [221] = MOUSE_BUTTON_NONE,        /* */
    [222] = MOUSE_BUTTON_NONE,        /* */
    [223] = MOUSE_BUTTON_NONE,        /* */
    [224] = MOUSE_BUTTON_NONE,        /* */
    [225] = MOUSE_BUTTON_NONE,        /* */
    [226] = MOUSE_BUTTON_NONE,        /* */
    [227] = MOUSE_BUTTON_NONE,        /* */
    [228] = MOUSE_BUTTON_NONE,        /* */
    [229] = MOUSE_BUTTON_NONE,        /* */
    [230] = MOUSE_BUTTON_NONE,        /* */
    [231] = MOUSE_BUTTON_NONE,        /* */
    [232] = MOUSE_BUTTON_NONE,        /* */
    [233] = MOUSE_BUTTON_NONE,        /* */
    [234] = MOUSE_BUTTON_NONE,        /* */
    [235] = MOUSE_BUTTON_NONE,        /* */
    [236] = MOUSE_BUTTON_NONE,        /* */
    [237] = MOUSE_BUTTON_NONE,        /* */
    [238] = MOUSE_BUTTON_NONE,        /* */
    [239] = MOUSE_BUTTON_NONE,        /* */
    [240] = MOUSE_BUTTON_NONE,        /* */
    [241] = MOUSE_BUTTON_NONE,        /* */
    [242] = MOUSE_BUTTON_NONE,        /* */
    [243] = MOUSE_BUTTON_NONE,        /* */
    [244] = MOUSE_BUTTON_NONE,        /* */
    [245] = MOUSE_BUTTON_NONE,        /* */
    [246] = MOUSE_BUTTON_NONE,        /* */
    [247] = MOUSE_BUTTON_NONE,        /* */
    [248] = MOUSE_BUTTON_NONE,        /* */
    [249] = MOUSE_BUTTON_NONE,        /* */
    [250] = MOUSE_BUTTON_NONE,        /* */
    [251] = MOUSE_BUTTON_NONE,        /* */
    [252] = MOUSE_BUTTON_NONE,        /* */
    [253] = MOUSE_BUTTON_NONE,        /* */
    [254] = MOUSE_BUTTON_NONE,        /* */
    [255] = MOUSE_BUTTON_NONE,        /* */
};

static const mouse_scroll_direction_t platform_input_mouse_scroll_direction_mapping[PLATFORM_INPUT_MOUSE_BUTTON_COUNT] =
    {
        [0] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [1] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [2] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [3] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [4] = MOUSE_SCROLL_DIRECTION_UP,     /* */
        [5] = MOUSE_SCROLL_DIRECTION_DOWN,   /* */
        [6] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [7] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [8] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [9] = MOUSE_SCROLL_DIRECTION_NONE,   /* */
        [10] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [11] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [12] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [13] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [14] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [15] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [16] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [17] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [18] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [19] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [20] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [21] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [22] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [23] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [24] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [25] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [26] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [27] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [28] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [29] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [30] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [31] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [32] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [33] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [34] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [35] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [36] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [37] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [38] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [39] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [40] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [41] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [42] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [43] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [44] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [45] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [46] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [47] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [48] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [49] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [50] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [51] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [52] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [53] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [54] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [55] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [56] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [57] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [58] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [59] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [60] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [61] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [62] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [63] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [64] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [65] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [66] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [67] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [68] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [69] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [70] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [71] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [72] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [73] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [74] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [75] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [76] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [77] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [78] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [79] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [80] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [81] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [82] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [83] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [84] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [85] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [86] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [87] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [88] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [89] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [90] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [91] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [92] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [93] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [94] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [95] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [96] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [97] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [98] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [99] = MOUSE_SCROLL_DIRECTION_NONE,  /* */
        [100] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [101] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [102] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [103] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [104] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [105] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [106] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [107] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [108] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [109] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [110] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [111] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [112] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [113] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [114] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [115] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [116] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [117] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [118] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [119] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [120] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [121] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [122] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [123] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [124] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [125] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [126] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [127] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [128] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [129] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [130] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [131] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [132] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [133] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [134] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [135] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [136] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [137] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [138] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [139] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [140] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [141] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [142] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [143] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [144] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [145] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [146] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [147] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [148] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [149] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [150] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [151] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [152] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [153] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [154] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [155] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [156] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [157] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [158] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [159] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [160] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [161] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [162] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [163] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [164] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [165] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [166] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [167] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [168] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [169] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [170] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [171] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [172] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [173] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [174] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [175] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [176] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [177] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [178] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [179] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [180] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [181] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [182] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [183] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [184] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [185] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [186] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [187] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [188] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [189] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [190] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [191] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [192] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [193] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [194] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [195] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [196] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [197] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [198] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [199] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [200] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [201] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [202] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [203] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [204] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [205] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [206] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [207] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [208] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [209] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [210] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [211] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [212] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [213] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [214] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [215] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [216] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [217] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [218] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [219] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [220] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [221] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [222] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [223] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [224] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [225] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [226] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [227] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [228] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [229] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [230] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [231] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [232] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [233] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [234] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [235] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [236] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [237] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [238] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [239] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [240] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [241] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [242] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [243] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [244] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [245] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [246] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [247] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [248] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [249] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [250] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [251] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [252] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [253] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [254] = MOUSE_SCROLL_DIRECTION_NONE, /* */
        [255] = MOUSE_SCROLL_DIRECTION_NONE, /* */
};

static const mouse_scroll_state_t platform_input_mouse_scroll_direction_to_state_mapping[MOUSE_SCROLL_DIRECTION_COUNT] =
    {
        [MOUSE_SCROLL_DIRECTION_NONE] = MOUSE_SCROLL_STATE_NONE, /* */
        [MOUSE_SCROLL_DIRECTION_UP] = MOUSE_SCROLL_STATE_UP,     /* */
        [MOUSE_SCROLL_DIRECTION_DOWN] = MOUSE_SCROLL_STATE_DOWN, /* */
};

mouse_button_state_t __platform_input_mouse_button_states[MOUSE_BUTTON_COUNT] = {
    [MOUSE_BUTTON_NONE] = MOUSE_BUTTON_STATE_NONE,          /* */
    [MOUSE_BUTTON_LEFT] = MOUSE_BUTTON_STATE_NONE,          /* */
    [MOUSE_BUTTON_RIGHT] = MOUSE_BUTTON_STATE_NONE,         /* */
    [MOUSE_BUTTON_MIDDLE] = MOUSE_BUTTON_STATE_NONE,        /* */
    [MOUSE_BUTTON_SIDE_FORWARD] = MOUSE_BUTTON_STATE_NONE,  /* */
    [MOUSE_BUTTON_SIDE_BACKWARD] = MOUSE_BUTTON_STATE_NONE, /* */
};

mouse_scroll_state_t __platform_input_mouse_scroll_current_state = MOUSE_SCROLL_STATE_NONE;

mouse_button_t platform_input_mouse_button(xcb_button_t button) {
  return platform_input_mouse_button_mapping[button];
}

mouse_scroll_direction_t platform_input_mouse_scroll_direction(xcb_button_t button) {
  return platform_input_mouse_scroll_direction_mapping[button];
}

b8 platform_input_mouse_button_is_pressed(mouse_button_t button) {
  return __platform_input_mouse_button_state(button) == MOUSE_BUTTON_STATE_PRESSED;
}

b8 platform_input_mouse_button_is_released(mouse_button_t button) {
  return __platform_input_mouse_button_state(button) == MOUSE_BUTTON_STATE_RELEASED;
}

b8 platform_input_mouse_scroll_is_up() {
  return __platform_input_mouse_scroll_current_state == MOUSE_SCROLL_STATE_UP;
}

b8 platform_input_mouse_scroll_is_down() {
  return __platform_input_mouse_scroll_current_state == MOUSE_SCROLL_STATE_DOWN;
}

mouse_scroll_state_t platform_input_mouse_scroll_state() {
  return __platform_input_mouse_scroll_current_state;
}

mouse_button_state_t platform_input_mouse_button_state(mouse_button_t button) {
  return __platform_input_mouse_button_state(button);
}

b8 __platform_input_mouse_scroll_set_state(mouse_scroll_state_t state) {
  if (state < MOUSE_SCROLL_STATE_COUNT && state >= MOUSE_SCROLL_STATE_NONE) {
    __platform_input_mouse_scroll_current_state = state;

    return true;
  }

  return false;
}

b8 __platform_input_mouse_button_set_state(mouse_button_t button, mouse_button_state_t state) {
  if (button < MOUSE_BUTTON_COUNT && button >= MOUSE_BUTTON_NONE) {
    __platform_input_mouse_button_states[button] = state;

    return true;
  }

  return false;
}

b8 __platform_input_mouse_button_clear_state() {
  for (mouse_button_t button = MOUSE_BUTTON_NONE; button < MOUSE_BUTTON_COUNT; ++button) {
    if (__platform_input_mouse_button_states[button] == MOUSE_BUTTON_STATE_RELEASED) {
      __platform_input_mouse_button_states[button] = MOUSE_BUTTON_STATE_NONE;
    }
  }

  return true;
}

b8 __platform_input_mouse_scroll_clear_state() {
  __platform_input_mouse_scroll_current_state = MOUSE_SCROLL_STATE_NONE;

  return true;
}

mouse_scroll_state_t __platform_input_mouse_scroll_state() {
  return __platform_input_mouse_scroll_current_state;
}

mouse_button_state_t __platform_input_mouse_button_state(mouse_button_t button) {
  if (button < MOUSE_BUTTON_COUNT && button >= MOUSE_BUTTON_NONE) {
    return __platform_input_mouse_button_states[button];
  }

  return MOUSE_BUTTON_STATE_NONE;
}

mouse_scroll_state_t __platform_input_mouse_scroll_direction_to_state(mouse_scroll_direction_t direction) {
  if (direction < MOUSE_SCROLL_DIRECTION_COUNT && direction >= MOUSE_SCROLL_DIRECTION_NONE) {
    return platform_input_mouse_scroll_direction_to_state_mapping[direction];
  }

  return MOUSE_SCROLL_STATE_NONE;
}
