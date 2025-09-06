#include "core/ScriptObject.h"

namespace Moho {

enum EMauiEventType
{
    MET_MouseMotion = 0x1,
    MET_MouseEnter = 0x2,
    MET_MouseHover = 0x3,
    MET_MouseExit = 0x4,
    MET_ButtonPress = 0x5,
    MET_ButtonDClick = 0x6,
    MET_ButtonRelease = 0x7,
    MET_WheelRotation = 0x8,
    MET_KeyUp = 0x9,
    MET_KeyDown = 0xA,
    MET_Char = 0xB,
};

enum EMauiKeyCode
{
    MKEY_BACK = 0x8,
    MKEY_TAB = 0x9,
    MKEY_RETURN = 0xD,
    MKEY_ESCAPE = 0x1B,
    MKEY_SPACE = 0x20,
    /* MKEY_0 - MKEY_9, MKEY_A - MKEY_Z */
    MKEY_DELETE = 0x7F,
    MKEY_START = 0x12C,
    MKEY_LBUTTON = 0x12D,
    MKEY_RBUTTON = 0x12E,
    MKEY_CANCEL = 0x12F,
    MKEY_MBUTTON = 0x130,
    MKEY_CLEAR = 0x131,
    MKEY_SHIFT = 0x132,
    MKEY_ALT = 0x133,
    MKEY_CONTROL = 0x134,
    MKEY_MENU = 0x135,
    MKEY_PAUSE = 0x136,
    MKEY_CAPITAL = 0x137,
    MKEY_PRIOR = 0x138,
    MKEY_NEXT = 0x139,
    MKEY_END = 0x13A,
    MKEY_HOME = 0x13B,
    MKEY_LEFT = 0x13C,
    MKEY_UP = 0x13D,
    MKEY_RIGHT = 0x13E,
    MKEY_DOWN = 0x13F,
    MKEY_SELECT = 0x140,
    MKEY_PRINT = 0x141,
    MKEY_EXECUTE = 0x142,
    MKEY_SNAPSHOT = 0x143,
    MKEY_INSERT = 0x144,
    MKEY_HELP = 0x145,
    MKEY_NUMPAD0 = 0x146,
    MKEY_NUMPAD1 = 0x147,
    MKEY_NUMPAD2 = 0x148,
    MKEY_NUMPAD3 = 0x149,
    MKEY_NUMPAD4 = 0x14A,
    MKEY_NUMPAD5 = 0x14B,
    MKEY_NUMPAD6 = 0x14C,
    MKEY_NUMPAD7 = 0x14D,
    MKEY_NUMPAD8 = 0x14E,
    MKEY_NUMPAD9 = 0x14F,
    MKEY_MULTIPLY = 0x150,
    MKEY_ADD = 0x151,
    MKEY_SEPARATOR = 0x152,
    MKEY_SUBTRACT = 0x153,
    MKEY_DECIMAL = 0x154,
    MKEY_DIVIDE = 0x155,
    MKEY_F1 = 0x156,
    MKEY_F2 = 0x157,
    MKEY_F3 = 0x158,
    MKEY_F4 = 0x159,
    MKEY_F5 = 0x15A,
    MKEY_F6 = 0x15B,
    MKEY_F7 = 0x15C,
    MKEY_F8 = 0x15D,
    MKEY_F9 = 0x15E,
    MKEY_F10 = 0x15F,
    MKEY_F11 = 0x160,
    MKEY_F12 = 0x161,
    MKEY_F13 = 0x162,
    MKEY_F14 = 0x163,
    MKEY_F15 = 0x164,
    MKEY_F16 = 0x165,
    MKEY_F17 = 0x166,
    MKEY_F18 = 0x167,
    MKEY_F19 = 0x168,
    MKEY_F20 = 0x169,
    MKEY_F21 = 0x16A,
    MKEY_F22 = 0x16B,
    MKEY_F23 = 0x16C,
    MKEY_F24 = 0x16D,
    MKEY_NUMLOCK = 0x16E,
    MKEY_SCROLL = 0x16F,
    MKEY_PAGEUP = 0x170,
    MKEY_PAGEDOWN = 0x171,
    MKEY_NUMPAD_SPACE = 0x172,
    MKEY_NUMPAD_TAB = 0x173,
    MKEY_NUMPAD_ENTER = 0x174,
    MKEY_NUMPAD_F1 = 0x175,
    MKEY_NUMPAD_F2 = 0x176,
    MKEY_NUMPAD_F3 = 0x177,
    MKEY_NUMPAD_F4 = 0x178,
    MKEY_NUMPAD_HOME = 0x179,
    MKEY_NUMPAD_LEFT = 0x17A,
    MKEY_NUMPAD_UP = 0x17B,
    MKEY_NUMPAD_RIGHT = 0x17C,
    MKEY_NUMPAD_DOWN = 0x17D,
    MKEY_NUMPAD_PRIOR = 0x17E,
    MKEY_NUMPAD_PAGEUP = 0x17F,
    MKEY_NUMPAD_NEXT = 0x180,
    MKEY_NUMPAD_PAGEDOWN = 0x181,
    MKEY_NUMPAD_END = 0x182,
    MKEY_NUMPAD_BEGIN = 0x183,
    MKEY_NUMPAD_INSERT = 0x184,
    MKEY_NUMPAD_DELETE = 0x185,
    MKEY_NUMPAD_EQUAL = 0x186,
    MKEY_NUMPAD_MULTIPLY = 0x187,
    MKEY_NUMPAD_ADD = 0x188,
    MKEY_NUMPAD_SEPARATOR = 0x189,
    MKEY_NUMPAD_SUBTRACT = 0x18A,
    MKEY_NUMPAD_DECIMAL = 0x18B,
    MKEY_NUMPAD_DIVIDE = 0x18C,
};

enum EMauiEventModifier/*guess*/
{
    MEM_Shift = 0x1,
    MEM_Ctrl = 0x2,
    MEM_Alt = 0x4,
    MEM_Left = 0x10,
    MEM_Middle = 0x20,
    MEM_Right = 0x40,
};

struct SMauiEventData
{
    Moho::EMauiEventType mEventType;
    float mMouseX;
    float mMouseY;
    int mWheelRotation;
    int mWheelData;
    Moho::EMauiKeyCode mKeyCode;
    int mRawKeyCode;
    Moho::EMauiEventModifier mModifiers;
    Moho::CScriptObject *mSource;
};

}