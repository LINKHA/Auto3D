#pragma once

#include "../Math/IntVector2.h"
#include "../Object/Object.h"

#include <SDL_scancode.h>

namespace Auto3D
{
#define SCANCODE_MASK (1<<30)
#define TO_KEYCODE(X)  (X | SCANCODE_MASK)


enum ScancodeKey
{
	KEY_UNKNOWN = 0,

	KEY_RETURN = '\r',
	KEY_ESCAPE = '\033',
	KEY_BACKSPACE = '\b',
	KEY_TAB = '\t',
	KEY_SPACE = ' ',
	KEY_EXCLAIM = '!',
	KEY_QUOTEDBL = '"',
	KEY_HASH = '#',
	KEY_PERCENT = '%',
	KEY_DOLLAR = '$',
	KEY_AMPERSAND = '&',
	KEY_QUOTE = '\'',
	KEY_LEFTPAREN = '(',
	KEY_RIGHTPAREN = ')',
	KEY_ASTERISK = '*',
	KEY_PLUS = '+',
	KEY_COMMA = ',',
	KEY_MINUS = '-',
	KEY_PERIOD = '.',
	KEY_SLASH = '/',
	KEY_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',
	KEY_COLON = ':',
	KEY_SEMICOLON = ';',
	KEY_LESS = '<',
	KEY_EQUALS = '=',
	KEY_GREATER = '>',
	KEY_QUESTION = '?',
	KEY_AT = '@',
	/*
	Skip uppercase letters
	*/
	KEY_LEFTBRACKET = '[',
	KEY_BACKSLASH = '\\',
	KEY_RIGHTBRACKET = ']',
	KEY_CARET = '^',
	KEY_UNDERSCORE = '_',
	KEY_BACKQUOTE = '`',
	KEY_A = 'a',
	KEY_B = 'b',
	KEY_C = 'c',
	KEY_D = 'd',
	KEY_E = 'e',
	KEY_F = 'f',
	KEY_G = 'g',
	KEY_H = 'h',
	KEY_I = 'i',
	KEY_J = 'j',
	KEY_K = 'k',
	KEY_L = 'l',
	KEY_M = 'm',
	KEY_N = 'n',
	KEY_O = 'o',
	KEY_P = 'p',
	KEY_Q = 'q',
	KEY_R = 'r',
	KEY_S = 's',
	KEY_T = 't',
	KEY_U = 'u',
	KEY_V = 'v',
	KEY_W = 'w',
	KEY_X = 'x',
	KEY_Y = 'y',
	KEY_Z = 'z',

	KEY_CAPSLOCK = TO_KEYCODE(SDL_SCANCODE_CAPSLOCK),

	KEY_F1 = TO_KEYCODE(SDL_SCANCODE_F1),
	KEY_F2 = TO_KEYCODE(SDL_SCANCODE_F2),
	KEY_F3 = TO_KEYCODE(SDL_SCANCODE_F3),
	KEY_F4 = TO_KEYCODE(SDL_SCANCODE_F4),
	KEY_F5 = TO_KEYCODE(SDL_SCANCODE_F5),
	KEY_F6 = TO_KEYCODE(SDL_SCANCODE_F6),
	KEY_F7 = TO_KEYCODE(SDL_SCANCODE_F7),
	KEY_F8 = TO_KEYCODE(SDL_SCANCODE_F8),
	KEY_F9 = TO_KEYCODE(SDL_SCANCODE_F9),
	KEY_F10 = TO_KEYCODE(SDL_SCANCODE_F10),
	KEY_F11 = TO_KEYCODE(SDL_SCANCODE_F11),
	KEY_F12 = TO_KEYCODE(SDL_SCANCODE_F12),

	KEY_PRINTSCREEN = TO_KEYCODE(SDL_SCANCODE_PRINTSCREEN),
	KEY_SCROLLLOCK = TO_KEYCODE(SDL_SCANCODE_SCROLLLOCK),
	KEY_PAUSE = TO_KEYCODE(SDL_SCANCODE_PAUSE),
	KEY_INSERT = TO_KEYCODE(SDL_SCANCODE_INSERT),
	KEY_HOME = TO_KEYCODE(SDL_SCANCODE_HOME),
	KEY_PAGEUP = TO_KEYCODE(SDL_SCANCODE_PAGEUP),
	KEY_DELETE = '\177',
	KEY_END = TO_KEYCODE(SDL_SCANCODE_END),
	KEY_PAGEDOWN = TO_KEYCODE(SDL_SCANCODE_PAGEDOWN),
	KEY_RIGHT = TO_KEYCODE(SDL_SCANCODE_RIGHT),
	KEY_LEFT = TO_KEYCODE(SDL_SCANCODE_LEFT),
	KEY_DOWN = TO_KEYCODE(SDL_SCANCODE_DOWN),
	KEY_UP = TO_KEYCODE(SDL_SCANCODE_UP),

	KEY_NUMLOCKCLEAR = TO_KEYCODE(SDL_SCANCODE_NUMLOCKCLEAR),
	KEY_KP_DIVIDE = TO_KEYCODE(SDL_SCANCODE_KP_DIVIDE),
	KEY_KP_MULTIPLY = TO_KEYCODE(SDL_SCANCODE_KP_MULTIPLY),
	KEY_KP_MINUS = TO_KEYCODE(SDL_SCANCODE_KP_MINUS),
	KEY_KP_PLUS = TO_KEYCODE(SDL_SCANCODE_KP_PLUS),
	KEY_KP_ENTER = TO_KEYCODE(SDL_SCANCODE_KP_ENTER),
	KEY_KP_1 = TO_KEYCODE(SDL_SCANCODE_KP_1),
	KEY_KP_2 = TO_KEYCODE(SDL_SCANCODE_KP_2),
	KEY_KP_3 = TO_KEYCODE(SDL_SCANCODE_KP_3),
	KEY_KP_4 = TO_KEYCODE(SDL_SCANCODE_KP_4),
	KEY_KP_5 = TO_KEYCODE(SDL_SCANCODE_KP_5),
	KEY_KP_6 = TO_KEYCODE(SDL_SCANCODE_KP_6),
	KEY_KP_7 = TO_KEYCODE(SDL_SCANCODE_KP_7),
	KEY_KP_8 = TO_KEYCODE(SDL_SCANCODE_KP_8),
	KEY_KP_9 = TO_KEYCODE(SDL_SCANCODE_KP_9),
	KEY_KP_0 = TO_KEYCODE(SDL_SCANCODE_KP_0),
	KEY_KP_PERIOD = TO_KEYCODE(SDL_SCANCODE_KP_PERIOD),

	KEY_APPLICATION = TO_KEYCODE(SDL_SCANCODE_APPLICATION),
	KEY_POWER = TO_KEYCODE(SDL_SCANCODE_POWER),
	KEY_KP_EQUALS = TO_KEYCODE(SDL_SCANCODE_KP_EQUALS),
	KEY_F13 = TO_KEYCODE(SDL_SCANCODE_F13),
	KEY_F14 = TO_KEYCODE(SDL_SCANCODE_F14),
	KEY_F15 = TO_KEYCODE(SDL_SCANCODE_F15),
	KEY_F16 = TO_KEYCODE(SDL_SCANCODE_F16),
	KEY_F17 = TO_KEYCODE(SDL_SCANCODE_F17),
	KEY_F18 = TO_KEYCODE(SDL_SCANCODE_F18),
	KEY_F19 = TO_KEYCODE(SDL_SCANCODE_F19),
	KEY_F20 = TO_KEYCODE(SDL_SCANCODE_F20),
	KEY_F21 = TO_KEYCODE(SDL_SCANCODE_F21),
	KEY_F22 = TO_KEYCODE(SDL_SCANCODE_F22),
	KEY_F23 = TO_KEYCODE(SDL_SCANCODE_F23),
	KEY_F24 = TO_KEYCODE(SDL_SCANCODE_F24),

	KEY_KP_COMMA = TO_KEYCODE(SDL_SCANCODE_KP_COMMA),
	KEY_KP_EQUALSAS400 =
	TO_KEYCODE(SDL_SCANCODE_KP_EQUALSAS400),

	KEY_ALTERASE = TO_KEYCODE(SDL_SCANCODE_ALTERASE),
	KEY_SYSREQ = TO_KEYCODE(SDL_SCANCODE_SYSREQ),
	KEY_CANCEL = TO_KEYCODE(SDL_SCANCODE_CANCEL),
	KEY_CLEAR = TO_KEYCODE(SDL_SCANCODE_CLEAR),
	KEY_PRIOR = TO_KEYCODE(SDL_SCANCODE_PRIOR),
	KEY_RETURN2 = TO_KEYCODE(SDL_SCANCODE_RETURN2),
	KEY_SEPARATOR = TO_KEYCODE(SDL_SCANCODE_SEPARATOR),
	KEY_OUT = TO_KEYCODE(SDL_SCANCODE_OUT),
	KEY_OPER = TO_KEYCODE(SDL_SCANCODE_OPER),
	KEY_CLEARAGAIN = TO_KEYCODE(SDL_SCANCODE_CLEARAGAIN),
	KEY_CRSEL = TO_KEYCODE(SDL_SCANCODE_CRSEL),
	KEY_EXSEL = TO_KEYCODE(SDL_SCANCODE_EXSEL),

	KEY_KP_00 = TO_KEYCODE(SDL_SCANCODE_KP_00),
	KEY_KP_000 = TO_KEYCODE(SDL_SCANCODE_KP_000),
	KEY_THOUSANDSSEPARATOR =
	TO_KEYCODE(SDL_SCANCODE_THOUSANDSSEPARATOR),
	KEY_DECIMALSEPARATOR =
	TO_KEYCODE(SDL_SCANCODE_DECIMALSEPARATOR),
	KEY_CURRENCYUNIT = TO_KEYCODE(SDL_SCANCODE_CURRENCYUNIT),
	KEY_CURRENCYSUBUNIT =
	TO_KEYCODE(SDL_SCANCODE_CURRENCYSUBUNIT),
	KEY_KP_LEFTPAREN = TO_KEYCODE(SDL_SCANCODE_KP_LEFTPAREN),
	KEY_KP_RIGHTPAREN = TO_KEYCODE(SDL_SCANCODE_KP_RIGHTPAREN),
	KEY_KP_LEFTBRACE = TO_KEYCODE(SDL_SCANCODE_KP_LEFTBRACE),
	KEY_KP_RIGHTBRACE = TO_KEYCODE(SDL_SCANCODE_KP_RIGHTBRACE),
	KEY_KP_TAB = TO_KEYCODE(SDL_SCANCODE_KP_TAB),
	KEY_KP_BACKSPACE = TO_KEYCODE(SDL_SCANCODE_KP_BACKSPACE),
	KEY_KP_A = TO_KEYCODE(SDL_SCANCODE_KP_A),
	KEY_KP_B = TO_KEYCODE(SDL_SCANCODE_KP_B),
	KEY_KP_C = TO_KEYCODE(SDL_SCANCODE_KP_C),
	KEY_KP_D = TO_KEYCODE(SDL_SCANCODE_KP_D),
	KEY_KP_E = TO_KEYCODE(SDL_SCANCODE_KP_E),
	KEY_KP_F = TO_KEYCODE(SDL_SCANCODE_KP_F),
	KEY_KP_XOR = TO_KEYCODE(SDL_SCANCODE_KP_XOR),
	KEY_KP_POWER = TO_KEYCODE(SDL_SCANCODE_KP_POWER),
	KEY_KP_PERCENT = TO_KEYCODE(SDL_SCANCODE_KP_PERCENT),
	KEY_KP_LESS = TO_KEYCODE(SDL_SCANCODE_KP_LESS),
	KEY_KP_GREATER = TO_KEYCODE(SDL_SCANCODE_KP_GREATER),
	KEY_KP_AMPERSAND = TO_KEYCODE(SDL_SCANCODE_KP_AMPERSAND),
	KEY_KP_DBLAMPERSAND =
	TO_KEYCODE(SDL_SCANCODE_KP_DBLAMPERSAND),
	KEY_KP_VERTICALBAR =
	TO_KEYCODE(SDL_SCANCODE_KP_VERTICALBAR),
	KEY_KP_DBLVERTICALBAR =
	TO_KEYCODE(SDL_SCANCODE_KP_DBLVERTICALBAR),
	KEY_KP_COLON = TO_KEYCODE(SDL_SCANCODE_KP_COLON),
	KEY_KP_HASH = TO_KEYCODE(SDL_SCANCODE_KP_HASH),
	KEY_KP_SPACE = TO_KEYCODE(SDL_SCANCODE_KP_SPACE),
	KEY_KP_AT = TO_KEYCODE(SDL_SCANCODE_KP_AT),
	KEY_KP_EXCLAM = TO_KEYCODE(SDL_SCANCODE_KP_EXCLAM),
	KEY_KP_MEMSTORE = TO_KEYCODE(SDL_SCANCODE_KP_MEMSTORE),
	KEY_KP_MEMRECALL = TO_KEYCODE(SDL_SCANCODE_KP_MEMRECALL),
	KEY_KP_MEMCLEAR = TO_KEYCODE(SDL_SCANCODE_KP_MEMCLEAR),
	KEY_KP_MEMADD = TO_KEYCODE(SDL_SCANCODE_KP_MEMADD),
	KEY_KP_MEMSUBTRACT =
	TO_KEYCODE(SDL_SCANCODE_KP_MEMSUBTRACT),
	KEY_KP_MEMMULTIPLY =
	TO_KEYCODE(SDL_SCANCODE_KP_MEMMULTIPLY),
	KEY_KP_MEMDIVIDE = TO_KEYCODE(SDL_SCANCODE_KP_MEMDIVIDE),
	KEY_KP_PLUSMINUS = TO_KEYCODE(SDL_SCANCODE_KP_PLUSMINUS),
	KEY_KP_CLEAR = TO_KEYCODE(SDL_SCANCODE_KP_CLEAR),
	KEY_KP_CLEARENTRY = TO_KEYCODE(SDL_SCANCODE_KP_CLEARENTRY),
	KEY_KP_BINARY = TO_KEYCODE(SDL_SCANCODE_KP_BINARY),
	KEY_KP_OCTAL = TO_KEYCODE(SDL_SCANCODE_KP_OCTAL),
	KEY_KP_DECIMAL = TO_KEYCODE(SDL_SCANCODE_KP_DECIMAL),
	KEY_KP_HEXADECIMAL =
	TO_KEYCODE(SDL_SCANCODE_KP_HEXADECIMAL),

	KEY_LCTRL = TO_KEYCODE(SDL_SCANCODE_LCTRL),
	KEY_LSHIFT = TO_KEYCODE(SDL_SCANCODE_LSHIFT),
	KEY_LALT = TO_KEYCODE(SDL_SCANCODE_LALT),
	KEY_LGUI = TO_KEYCODE(SDL_SCANCODE_LGUI),
	KEY_RCTRL = TO_KEYCODE(SDL_SCANCODE_RCTRL),
	KEY_RSHIFT = TO_KEYCODE(SDL_SCANCODE_RSHIFT),
	KEY_RALT = TO_KEYCODE(SDL_SCANCODE_RALT),
	KEY_RGUI = TO_KEYCODE(SDL_SCANCODE_RGUI),

	KEY_MODE = TO_KEYCODE(SDL_SCANCODE_MODE),

	KEY_AUDIONEXT = TO_KEYCODE(SDL_SCANCODE_AUDIONEXT),
	KEY_AUDIOPREV = TO_KEYCODE(SDL_SCANCODE_AUDIOPREV),
	KEY_AUDIOSTOP = TO_KEYCODE(SDL_SCANCODE_AUDIOSTOP),
	KEY_AUDIOPLAY = TO_KEYCODE(SDL_SCANCODE_AUDIOPLAY),
	KEY_AUDIOMUTE = TO_KEYCODE(SDL_SCANCODE_AUDIOMUTE),
	KEY_MEDIASELECT = TO_KEYCODE(SDL_SCANCODE_MEDIASELECT),
	KEY_WWW = TO_KEYCODE(SDL_SCANCODE_WWW),
	KEY_MAIL = TO_KEYCODE(SDL_SCANCODE_MAIL),
	KEY_CALCULATOR = TO_KEYCODE(SDL_SCANCODE_CALCULATOR),
	KEY_COMPUTER = TO_KEYCODE(SDL_SCANCODE_COMPUTER),
	KEY_AC_SEARCH = TO_KEYCODE(SDL_SCANCODE_AC_SEARCH),
	KEY_AC_HOME = TO_KEYCODE(SDL_SCANCODE_AC_HOME),
	KEY_AC_BACK = TO_KEYCODE(SDL_SCANCODE_AC_BACK),
	KEY_AC_FORWARD = TO_KEYCODE(SDL_SCANCODE_AC_FORWARD),
	KEY_AC_STOP = TO_KEYCODE(SDL_SCANCODE_AC_STOP),
	KEY_AC_REFRESH = TO_KEYCODE(SDL_SCANCODE_AC_REFRESH),
	KEY_AC_BOOKMARKS = TO_KEYCODE(SDL_SCANCODE_AC_BOOKMARKS),

	KEY_BRIGHTNESSDOWN =
	TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSDOWN),
	KEY_BRIGHTNESSUP = TO_KEYCODE(SDL_SCANCODE_BRIGHTNESSUP),
	KEY_DISPLAYSWITCH = TO_KEYCODE(SDL_SCANCODE_DISPLAYSWITCH),
	KEY_KBDILLUMTOGGLE =
	TO_KEYCODE(SDL_SCANCODE_KBDILLUMTOGGLE),
	KEY_KBDILLUMDOWN = TO_KEYCODE(SDL_SCANCODE_KBDILLUMDOWN),
	KEY_KBDILLUMUP = TO_KEYCODE(SDL_SCANCODE_KBDILLUMUP),
	KEY_EJECT = TO_KEYCODE(SDL_SCANCODE_EJECT),
	KEY_SLEEP = TO_KEYCODE(SDL_SCANCODE_SLEEP),
	KEY_APP1 = TO_KEYCODE(SDL_SCANCODE_APP1),
	KEY_APP2 = TO_KEYCODE(SDL_SCANCODE_APP2),

	KEY_AUDIOREWIND = TO_KEYCODE(SDL_SCANCODE_AUDIOREWIND),
	KEY_AUDIOFASTFORWARD = TO_KEYCODE(SDL_SCANCODE_AUDIOFASTFORWARD)
};

/// Left mouse button index.
static const unsigned MOUSEB_LEFT = 0;
/// Middle mouse button index.
static const unsigned MOUSEB_MIDDLE = 1;
/// Right mouse button index.
static const unsigned MOUSEB_RIGHT = 2;

/// Finger touch.
struct AUTO_API Touch
{
    /// Construct.
    Touch() :
        delta(IntVector2::ZERO),
        lastDelta(IntVector2::ZERO)
    {
    }

    /// Zero-based touch id.
    unsigned id;
    /// Operating system id, which may be an arbitrary number.
    unsigned internalId;
    /// Position within window.
    IntVector2 position;
    /// Accumulated delta on this frame.
    IntVector2 delta;
    /// Delta from last move event.
    IntVector2 lastDelta;
    /// Current finger pressure.
    float pressure;
};

/// Key press or release event.
class AUTO_API KeyEvent : public Event
{
public:
    /// Key code.
    unsigned keyCode;
    /// Raw key code.
    unsigned rawKeyCode;
    /// Pressed flag.
    bool pressed;
    /// Repeat flag.
    bool repeat;
};

/// Unicode character input event.
class AUTO_API CharInputEvent : public Event
{
public:
    /// Unicode codepoint.
    unsigned unicodeChar;
};

/// Mouse button press or release event.
class AUTO_API MouseButtonEvent : public Event
{
public:
    /// Button index.
    unsigned button;
    /// Bitmask of currently held down buttons.
    unsigned buttons;
    /// Pressed flag.
    bool pressed;
    /// Mouse position within window.
    IntVector2 position;
};

/// Mouse move event.
class AUTO_API MouseMoveEvent : public Event
{
public:
    /// Bitmask of currently held down buttons.
    unsigned buttons;
    /// Mouse position within window.
    IntVector2 position;
    /// Delta from last position.
    IntVector2 delta;
};

/// Touch begin event.
class AUTO_API TouchBeginEvent : public Event
{
public:
    /// Zero-based touch id.
    unsigned id;
    /// Touch position within window.
    IntVector2 position;
    /// Finger pressure between 0-1.
    float pressure;
};

/// Touch move event.
class AUTO_API TouchMoveEvent : public Event
{
public:
    /// Zero-based touch id.
    unsigned id;
    /// Touch position within window.
    IntVector2 position;
    /// Delta from last position.
    IntVector2 delta;
    /// Finger pressure between 0-1.
    float pressure;
};

/// Touch end event.
class AUTO_API TouchEndEvent : public Event
{
public:
    /// Zero-based touch id.
    unsigned id;
    /// Touch position within window.
    IntVector2 position;
};

/// %Input subsystem for reading keyboard/mouse/etc. input. Updated from OS window messages by the Window class.
class AUTO_API Input : public Object
{
    REGISTER_OBJECT_CLASS(Input, Object)

public:
    /// Construct and register subsystem.
    Input();
    /// Destruct.
    ~Input();

    /// Poll the window (if any) for OS window messages and update input state.
    void Update();

    /// Return whether key is down by key code.
    bool IsKeyDown(unsigned keyCode) const;
    /// Return whether key is down by raw key code.
    bool IsKeyDownRaw(unsigned rawKeyCode) const;
    /// Return whether key was pressed on this frame by key code.
    bool IsKeyPress(unsigned keyCode) const;
    /// Return whether key was pressed on this frame by raw key code.
    bool IsKeyPressRaw(unsigned rawKeyCode) const;
    /// Return current mouse position.
    const IntVector2& MousePosition() const;
    /// Return accumulated mouse movement since last frame.
    IntVector2 MouseMove() const { return mouseMove; }
    /// Return pressed down mouse buttons bitmask.
    unsigned MouseButtons() const { return mouseButtons; }
    /// Return whether a mouse button is down.
    bool IsMouseButtonDown(unsigned button) const;
    /// Return whether a mouse button was pressed on this frame.
    bool IsMouseButtonPress(unsigned button) const;
    /// Return number of active touches.
    size_t NumTouches() const { return touches.Size(); }
    /// Return an active touch by id, or null if not found.
    const Touch* FindTouch(unsigned id) const;
    /// Return all touches.
    const Vector<Touch>& Touches() const { return touches; }

    /// React to a key press or release. Called by window message handling.
    void OnKey(unsigned keyCode, unsigned rawKeyCode, bool pressed);
    /// React to char input. Called by window message handling.
    void OnChar(unsigned unicodeChar);
    /// React to a mouse move. Called by window message handling.
    void OnMouseMove(const IntVector2& position, const IntVector2& delta);

	void OnMouseWheel(const IntVector2& delta);
    /// React to a mouse button. Called by window message handling.
    void OnMouseButton(unsigned button, bool pressed);
    /// React to a touch. Called by window message handling.
    void OnTouch(unsigned internalId, bool pressed, const IntVector2& position, float pressure);
    /// React to gaining focus. Called by window message handling.
    void OnGainFocus();
    /// React to losing focus. Called by window message handling.
    void OnLoseFocus();

    /// Key press/release event.
    KeyEvent keyEvent;
    /// Unicode char input event.
    CharInputEvent charInputEvent;
    /// Mouse button press/release event.
    MouseButtonEvent mouseButtonEvent;
    /// Mouse move event.
    MouseMoveEvent mouseMoveEvent;
    /// Touch begin event.
    TouchBeginEvent touchBeginEvent;
    /// Touch move event.
    TouchMoveEvent touchMoveEvent;
    /// Touch end event.
    TouchEndEvent touchEndEvent;

private:
    /// Key code held down status.
    HashMap<unsigned, bool> keyDown;
    /// Key code pressed status.
    HashMap<unsigned, bool> keyPressed;
    /// Raw key code held down status.
    HashMap<unsigned, bool> rawKeyDown;
    /// Raw key code pressed status.
    HashMap<unsigned, bool> rawKeyPress;
    /// Active touches.
    Vector<Touch> touches;
    /// Accumulated mouse move since last frame.
    IntVector2 mouseMove;

	IntVector2 mouseWhellOffset;
    /// Mouse buttons bitmask.
    unsigned mouseButtons;
    /// Mouse buttons pressed bitmask.
    unsigned mouseButtonsPressed;
};

}
