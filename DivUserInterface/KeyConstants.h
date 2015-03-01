#define DIV_KEY_ESCAPE          0x01
#define DIV_KEY_1               0x02
#define DIV_KEY_2               0x03
#define DIV_KEY_3               0x04
#define DIV_KEY_4               0x05
#define DIV_KEY_5               0x06
#define DIV_KEY_6               0x07
#define DIV_KEY_7               0x08
#define DIV_KEY_8               0x09
#define DIV_KEY_9               0x0A
#define DIV_KEY_0               0x0B
#define DIV_KEY_MINUS           0x0C    /* - on main keyboard */
#define DIV_KEY_EQUALS          0x0D
#define DIV_KEY_BACK            0x0E    /* backspace */
#define DIV_KEY_TAB             0x0F
#define DIV_KEY_Q               0x10
#define DIV_KEY_W               0x11
#define DIV_KEY_E               0x12
#define DIV_KEY_R               0x13
#define DIV_KEY_T               0x14
#define DIV_KEY_Y               0x15
#define DIV_KEY_U               0x16
#define DIV_KEY_I               0x17
#define DIV_KEY_O               0x18
#define DIV_KEY_P               0x19
#define DIV_KEY_LBRACKET        0x1A
#define DIV_KEY_RBRACKET        0x1B
#define DIV_KEY_RETURN          0x1C    /* Enter on main keyboard */
#define DIV_KEY_LCONTROL        0x1D
#define DIV_KEY_A               0x1E
#define DIV_KEY_S               0x1F
#define DIV_KEY_D               0x20
#define DIV_KEY_F               0x21
#define DIV_KEY_G               0x22
#define DIV_KEY_H               0x23
#define DIV_KEY_J               0x24
#define DIV_KEY_K               0x25
#define DIV_KEY_L               0x26
#define DIV_KEY_SEMICOLON       0x27
#define DIV_KEY_APOSTROPHE      0x28
#define DIV_KEY_GRAVE           0x29    /* accent grave */
#define DIV_KEY_LSHIFT          0x2A
#define DIV_KEY_BACKSLASH       0x2B
#define DIV_KEY_Z               0x2C
#define DIV_KEY_X               0x2D
#define DIV_KEY_C               0x2E
#define DIV_KEY_V               0x2F
#define DIV_KEY_B               0x30
#define DIV_KEY_N               0x31
#define DIV_KEY_M               0x32
#define DIV_KEY_COMMA           0x33
#define DIV_KEY_PERIOD          0x34    /* . on main keyboard */
#define DIV_KEY_SLASH           0x35    /* / on main keyboard */
#define DIV_KEY_RSHIFT          0x36
#define DIV_KEY_MULTIPLY        0x37    /* * on numeric keypad */
#define DIV_KEY_LMENU           0x38    /* left Alt */
#define DIV_KEY_SPACE           0x39
#define DIV_KEY_CAPITAL         0x3A
#define DIV_KEY_F1              0x3B
#define DIV_KEY_F2              0x3C
#define DIV_KEY_F3              0x3D
#define DIV_KEY_F4              0x3E
#define DIV_KEY_F5              0x3F
#define DIV_KEY_F6              0x40
#define DIV_KEY_F7              0x41
#define DIV_KEY_F8              0x42
#define DIV_KEY_F9              0x43
#define DIV_KEY_F10             0x44
#define DIV_KEY_NUMLOCK         0x45
#define DIV_KEY_SCROLL          0x46    /* Scroll Lock */
#define DIV_KEY_NUMPAD7         0x47
#define DIV_KEY_NUMPAD8         0x48
#define DIV_KEY_NUMPAD9         0x49
#define DIV_KEY_SUBTRACT        0x4A    /* - on numeric keypad */
#define DIV_KEY_NUMPAD4         0x4B
#define DIV_KEY_NUMPAD5         0x4C
#define DIV_KEY_NUMPAD6         0x4D
#define DIV_KEY_ADD             0x4E    /* + on numeric keypad */
#define DIV_KEY_NUMPAD1         0x4F
#define DIV_KEY_NUMPAD2         0x50
#define DIV_KEY_NUMPAD3         0x51
#define DIV_KEY_NUMPAD0         0x52
#define DIV_KEY_DECIMAL         0x53    /* . on numeric keypad */

#define DIV_KEY_F11             0x57
#define DIV_KEY_F12             0x58
#define DIV_KEY_F13             0x64    /*                     (NEC PC98) */
#define DIV_KEY_F14             0x65    /*                     (NEC PC98) */
#define DIV_KEY_F15             0x66    /*                     (NEC PC98) */

#define DIV_KEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */

#define DIV_KEY_AT              0x91    /*                     (NEC PC98) */
#define DIV_KEY_COLON           0x92    /*                     (NEC PC98) */
#define DIV_KEY_UNDERLINE       0x93    /*                     (NEC PC98) */

#define DIV_KEY_STOP            0x95    /*                     (NEC PC98) */

#define DIV_KEY_UNLABELED       0x97    /*                        (J3100) */
#define DIV_KEY_NEXTTRACK       0x99    /* Next Track */
#define DIV_KEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define DIV_KEY_RCONTROL        0x9D
#define DIV_KEY_MUTE            0xA0    /* Mute */
#define DIV_KEY_CALCULATOR      0xA1    /* Calculator */
#define DIV_KEY_PLAYPAUSE       0xA2    /* Play / Pause */
#define DIV_KEY_MEDIASTOP       0xA4    /* Media Stop */
#define DIV_KEY_VOLUMEDOWN      0xAE    /* Volume - */
#define DIV_KEY_VOLUMEUP        0xB0    /* Volume + */
#define DIV_KEY_WEBHOME         0xB2    /* Web home */
#define DIV_KEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define DIV_KEY_DIVIDE          0xB5    /* / on numeric keypad */
#define DIV_KEY_SYSRQ           0xB7
#define DIV_KEY_RMENU           0xB8    /* right Alt */
#define DIV_KEY_PAUSE           0xC5    /* Pause */
#define DIV_KEY_HOME            0xC7    /* Home on arrow keypad */
#define DIV_KEY_UP              0xC8    /* UpArrow on arrow keypad */
#define DIV_KEY_PRIOR           0xC9    /* PgUp on arrow keypad */
#define DIV_KEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define DIV_KEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define DIV_KEY_END             0xCF    /* End on arrow keypad */
#define DIV_KEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define DIV_KEY_NEXT            0xD1    /* PgDn on arrow keypad */
#define DIV_KEY_INSERT          0xD2    /* Insert on arrow keypad */
#define DIV_KEY_DELETE          0xD3    /* Delete on arrow keypad */
#define DIV_KEY_LWIN            0xDB    /* Left Windows key */
#define DIV_KEY_RWIN            0xDC    /* Right Windows key */
#define DIV_KEY_APPS            0xDD    /* AppMenu key */
#define DIV_KEY_POWER           0xDE    /* System Power */
#define DIV_KEY_SLEEP           0xDF    /* System Sleep */
#define DIV_KEY_WAKE            0xE3    /* System Wake */
#define DIV_KEY_WEBSEARCH       0xE5    /* Web Search */
#define DIV_KEY_WEBFAVORITES    0xE6    /* Web Favorites */
#define DIV_KEY_WEBREFRESH      0xE7    /* Web Refresh */
#define DIV_KEY_WEBSTOP         0xE8    /* Web Stop */
#define DIV_KEY_WEBFORWARD      0xE9    /* Web Forward */
#define DIV_KEY_WEBBACK         0xEA    /* Web Back */
#define DIV_KEY_MYCOMPUTER      0xEB    /* My Computer */
#define DIV_KEY_MAIL            0xEC    /* Mail */
#define DIV_KEY_MEDIASELECT     0xED    /* Media Select */

/*
 *  Alternate names for keys, to facilitate transition from DOS.
 */
#define DIV_KEY_BACKSPACE       DIV_KEY_BACK            /* backspace */
#define DIV_KEY_NUMPADSTAR      DIV_KEY_MULTIPLY        /* * on numeric keypad */
#define DIV_KEY_LALT            DIV_KEY_LMENU           /* left Alt */
#define DIV_KEY_CAPSLOCK        DIV_KEY_CAPITAL         /* CapsLock */
#define DIV_KEY_NUMPADMINUS     DIV_KEY_SUBTRACT        /* - on numeric keypad */
#define DIV_KEY_NUMPADPLUS      DIV_KEY_ADD             /* + on numeric keypad */
#define DIV_KEY_NUMPADPERIOD    DIV_KEY_DECIMAL         /* . on numeric keypad */
#define DIV_KEY_NUMPADSLASH     DIV_KEY_DIVIDE          /* / on numeric keypad */
#define DIV_KEY_RALT            DIV_KEY_RMENU           /* right Alt */
#define DIV_KEY_UPARROW         DIV_KEY_UP              /* UpArrow on arrow keypad */
#define DIV_KEY_PGUP            DIV_KEY_PRIOR           /* PgUp on arrow keypad */
#define DIV_KEY_LEFTARROW       DIV_KEY_LEFT            /* LeftArrow on arrow keypad */
#define DIV_KEY_RIGHTARROW      DIV_KEY_RIGHT           /* RightArrow on arrow keypad */
#define DIV_KEY_DOWNARROW       DIV_KEY_DOWN            /* DownArrow on arrow keypad */
#define DIV_KEY_PGDN            DIV_KEY_NEXT            /* PgDn on arrow keypad */

// These dont actually map to keys, but are used for mappping the mouse keys  =)

#define DIV_KEY_MOUSE1_DOWN			0xEE    
#define DIV_KEY_MOUSE1_UP			0xEF
#define DIV_KEY_MOUSE2_DOWN			0xF0
#define DIV_KEY_MOUSE2_UP			0xF1
#define DIV_KEY_MOUSE3_DOWN			0xF2
#define DIV_KEY_MOUSE3_UP			0xF3
#define DIV_KEY_MOUSE4_DOWN			0xF4
#define DIV_KEY_MOUSE4_UP			0xF5
#define DIV_KEY_MOUSESCROLL_DOWN    0xF6
#define DIV_KEY_MOUSESCROLL_UP		0xF7