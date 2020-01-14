#pragma once

#define TINYSTL_ALLOCATOR Auto3D::TinyStlAllocator

#include <bx/spscqueue.h>
#include <bx/filepath.h>

#ifndef ENTRY_CONFIG_USE_NOOP
#	define ENTRY_CONFIG_USE_NOOP 0
#endif // ENTRY_CONFIG_USE_NOOP

#ifndef ENTRY_CONFIG_USE_SDL
#	define ENTRY_CONFIG_USE_SDL 1
#endif // ENTRY_CONFIG_USE_SDL

#ifndef ENTRY_CONFIG_USE_GLFW
#	define ENTRY_CONFIG_USE_GLFW 0
#endif // ENTRY_CONFIG_USE_GLFW

#ifndef ENTRY_CONFIG_USE_WAYLAND
#	define ENTRY_CONFIG_USE_WAYLAND 0
#endif // ENTRY_CONFIG_USE_WAYLAND

#if !defined(ENTRY_CONFIG_USE_NATIVE) \
	&& !ENTRY_CONFIG_USE_NOOP \
	&& !ENTRY_CONFIG_USE_SDL \
	&& !ENTRY_CONFIG_USE_GLFW
#	define ENTRY_CONFIG_USE_NATIVE 1
#else
#	define ENTRY_CONFIG_USE_NATIVE 0
#endif // ...

#ifndef ENTRY_CONFIG_MAX_WINDOWS
#	define ENTRY_CONFIG_MAX_WINDOWS 8
#endif // ENTRY_CONFIG_MAX_WINDOWS

#ifndef ENTRY_CONFIG_MAX_GAMEPADS
#	define ENTRY_CONFIG_MAX_GAMEPADS 4
#endif // ENTRY_CONFIG_MAX_GAMEPADS

#if !defined(ENTRY_DEFAULT_WIDTH) && !defined(ENTRY_DEFAULT_HEIGHT)
#	define ENTRY_DEFAULT_WIDTH  1280
#	define ENTRY_DEFAULT_HEIGHT 720
#elif !defined(ENTRY_DEFAULT_WIDTH) || !defined(ENTRY_DEFAULT_HEIGHT)
#	error "Both ENTRY_DEFAULT_WIDTH and ENTRY_DEFAULT_HEIGHT must be defined."
#endif // ENTRY_DEFAULT_WIDTH

#ifndef ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
#	define ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR 1
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

#ifndef ENTRY_CONFIG_PROFILER
#	define ENTRY_CONFIG_PROFILER 0
#endif // ENTRY_CONFIG_PROFILER

#define ENTRY_IMPLEMENT_EVENT(_class, _type) \
			_class(WindowHandle _handle) : Event(_type, _handle) {}

namespace bx { struct FileReaderI; struct FileWriterI; struct AllocatorI; }

#define ENTRY_WINDOW_FLAG_NONE         UINT32_C(0x00000000)
#define ENTRY_WINDOW_FLAG_ASPECT_RATIO UINT32_C(0x00000001)
#define ENTRY_WINDOW_FLAG_FRAME        UINT32_C(0x00000002)

namespace Auto3D
{
struct WindowHandle { uint16_t idx; };
inline bool isValid(WindowHandle _handle) { return UINT16_MAX != _handle.idx; }


struct GamepadHandle { uint16_t idx; };
inline bool isValid(GamepadHandle _handle) { return UINT16_MAX != _handle.idx; }

enum SDL_USER_WINDOW
{
	SDL_USER_WINDOW_CREATE,
	SDL_USER_WINDOW_DESTROY,
	SDL_USER_WINDOW_SET_TITLE,
	SDL_USER_WINDOW_SET_FLAGS,
	SDL_USER_WINDOW_SET_POS,
	SDL_USER_WINDOW_SET_SIZE,
	SDL_USER_WINDOW_TOGGLE_FRAME,
	SDL_USER_WINDOW_TOGGLE_FULL_SCREEN,
	SDL_USER_WINDOW_MOUSE_LOCK,
};

struct MouseButton
{
	enum Enum
	{
		None,
		Left,
		Middle,
		Right,

		Count
	};
};

struct GamepadAxis
{
	enum Enum
	{
		LeftX,
		LeftY,
		LeftZ,
		RightX,
		RightY,
		RightZ,

		Count
	};
};

struct Modifier
{
	enum Enum
	{
		None = 0,
		LeftAlt = 0x01,
		RightAlt = 0x02,
		LeftCtrl = 0x04,
		RightCtrl = 0x08,
		LeftShift = 0x10,
		RightShift = 0x20,
		LeftMeta = 0x40,
		RightMeta = 0x80,
	};
};

struct Key
{
	enum Enum
	{
		None = 0,
		Esc,
		Return,
		Tab,
		Space,
		Backspace,
		Up,
		Down,
		Left,
		Right,
		Insert,
		Delete,
		Home,
		End,
		PageUp,
		PageDown,
		Print,
		Plus,
		Minus,
		LeftBracket,
		RightBracket,
		Semicolon,
		Quote,
		Comma,
		Period,
		Slash,
		Backslash,
		Tilde,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,
		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
		KeyA,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,

		GamepadA,
		GamepadB,
		GamepadX,
		GamepadY,
		GamepadThumbL,
		GamepadThumbR,
		GamepadShoulderL,
		GamepadShoulderR,
		GamepadUp,
		GamepadDown,
		GamepadLeft,
		GamepadRight,
		GamepadBack,
		GamepadStart,
		GamepadGuide,

		Count
	};
};

struct Suspend
{
	enum Enum
	{
		WillSuspend,
		DidSuspend,
		WillResume,
		DidResume,

		Count
	};
};
struct MouseState
{
	MouseState()
		: _mx(0)
		, _my(0)
		, _mz(0)
	{
		for (uint32_t ii = 0; ii < Auto3D::MouseButton::Count; ++ii)
		{
			_buttons[ii] = Auto3D::MouseButton::None;
		}
	}

	int32_t _mx;
	int32_t _my;
	int32_t _mz;
	uint8_t _buttons[Auto3D::MouseButton::Count];
};

struct GamepadState
{
	GamepadState()
	{
		bx::memSet(_axis, 0, sizeof(_axis));
	}

	int32_t _axis[Auto3D::GamepadAxis::Count];
};

bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _debug, uint32_t& _reset, MouseState* _mouse = NULL);

bx::FileReaderI* getFileReader();
bx::FileWriterI* getFileWriter();
bx::AllocatorI*  getAllocator();

struct FPlatform
{
	static WindowHandle CreateWindowHandle(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags = ENTRY_WINDOW_FLAG_NONE, const char* _title = "");
	static void DestroyWindowHandle(WindowHandle _handle);
	static void SetWindowPos(WindowHandle _handle, int32_t _x, int32_t _y);
	static void SetWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height);
	static void SetWindowTitle(WindowHandle _handle, const char* _title);
	static void SetWindowFlags(WindowHandle _handle, uint32_t _flags, bool _enabled);
	static void ToggleFullscreen(WindowHandle _handle);
	static void SetMouseLock(WindowHandle _handle, bool _lock);
	static void SetCurrentDir(const char* _dir);
};


struct WindowState
{
	WindowState()
		: _width(0)
		, _height(0)
		, _nwh(NULL)
	{
		_handle.idx = UINT16_MAX;
	}

	WindowHandle _handle;
	uint32_t     _width;
	uint32_t     _height;
	MouseState   _mouse;
	void*        _nwh;
	bx::FilePath _dropFile;
};

bool processWindowEvents(WindowState& state, uint32_t& debug, uint32_t& reset);

struct TinyStlAllocator
{
	static void* static_allocate(size_t bytes);
	static void static_deallocate(void* ptr, size_t /*_bytes*/);
};

int RunMain(int argc, const char* const* argv);

char keyToAscii(Key::Enum key, uint8_t modifiers);

struct Event
{
	enum Enum
	{
		Axis,
		Char,
		Exit,
		Gamepad,
		Key,
		Mouse,
		Size,
		Window,
		Suspend,
		DropFile,
	};

	Event(Enum type)
		: _type(type)
	{
		_handle.idx = UINT16_MAX;
	}

	Event(Enum _type, WindowHandle _handle)
		: _type(_type)
		, _handle(_handle)
	{
	}

	Event::Enum _type;
	WindowHandle _handle;
};

struct AxisEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(AxisEvent, Event::Axis);

	GamepadAxis::Enum _axis;
	int32_t _value;
	GamepadHandle _gamepad;
};

struct CharEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(CharEvent, Event::Char);

	uint8_t _len;
	uint8_t _char[4];
};

struct GamepadEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(GamepadEvent, Event::Gamepad);

	GamepadHandle _gamepad;
	bool _connected;
};

struct KeyEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(KeyEvent, Event::Key);

	Key::Enum _key;
	uint8_t _modifiers;
	bool _down;
};

struct MouseEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(MouseEvent, Event::Mouse);

	int32_t _mx;
	int32_t _my;
	int32_t _mz;
	MouseButton::Enum _button;
	bool _down;
	bool _move;
};

struct SizeEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(SizeEvent, Event::Size);

	uint32_t _width;
	uint32_t _height;
};

struct WindowEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(WindowEvent, Event::Window);

	void* _nwh;
};

struct SuspendEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(SuspendEvent, Event::Suspend);

	Suspend::Enum _state;
};

struct DropFileEvent : public Event
{
	ENTRY_IMPLEMENT_EVENT(DropFileEvent, Event::DropFile);

	bx::FilePath _filePath;
};

const Event* Poll();
const Event* Poll(WindowHandle handle);
void Release(const Event* event);

class EventQueue
{
public:
	EventQueue()
		: _queue(getAllocator() )
	{
	}

	~EventQueue()
	{
		for (const Event* ev = Poll(); NULL != ev; ev = Poll() )
		{
			Release(ev);
		}
	}

	void PostAxisEvent(WindowHandle _handle, GamepadHandle _gamepad, GamepadAxis::Enum _axis, int32_t _value)
	{
		AxisEvent* ev = BX_NEW(getAllocator(), AxisEvent)(_handle);
		ev->_gamepad = _gamepad;
		ev->_axis    = _axis;
		ev->_value   = _value;
		_queue.push(ev);
	}

	void PostCharEvent(WindowHandle _handle, uint8_t _len, const uint8_t _char[4])
	{
		CharEvent* ev = BX_NEW(getAllocator(), CharEvent)(_handle);
		ev->_len = _len;
		bx::memCopy(ev->_char, _char, 4);
		_queue.push(ev);
	}

	void PostExitEvent()
	{
		Event* ev = BX_NEW(getAllocator(), Event)(Event::Exit);
		_queue.push(ev);
	}

	void PostGamepadEvent(WindowHandle _handle, GamepadHandle _gamepad, bool _connected)
	{
		GamepadEvent* ev = BX_NEW(getAllocator(), GamepadEvent)(_handle);
		ev->_gamepad   = _gamepad;
		ev->_connected = _connected;
		_queue.push(ev);
	}

	void PostKeyEvent(WindowHandle handle, Key::Enum key, uint8_t modifiers, bool down)
	{
		KeyEvent* ev = BX_NEW(getAllocator(), KeyEvent)(handle);
		ev->_key       = key;
		ev->_modifiers = modifiers;
		ev->_down      = down;
		_queue.push(ev);
	}

	void PostMouseEvent(WindowHandle handle, int32_t mx, int32_t my, int32_t mz)
	{
		MouseEvent* ev = BX_NEW(getAllocator(), MouseEvent)(handle);
		ev->_mx     = mx;
		ev->_my     = my;
		ev->_mz     = mz;
		ev->_button = MouseButton::None;
		ev->_down   = false;
		ev->_move   = true;
		_queue.push(ev);
	}

	void PostMouseEvent(WindowHandle _handle, int32_t _mx, int32_t _my, int32_t _mz, MouseButton::Enum _button, bool _down)
	{
		MouseEvent* ev = BX_NEW(getAllocator(), MouseEvent)(_handle);
		ev->_mx     = _mx;
		ev->_my     = _my;
		ev->_mz     = _mz;
		ev->_button = _button;
		ev->_down   = _down;
		ev->_move   = false;
		_queue.push(ev);
	}

	void PostSizeEvent(WindowHandle _handle, uint32_t _width, uint32_t _height)
	{
		SizeEvent* ev = BX_NEW(getAllocator(), SizeEvent)(_handle);
		ev->_width  = _width;
		ev->_height = _height;
		_queue.push(ev);
	}

	void PostWindowEvent(WindowHandle handle, void* nwh = NULL)
	{
		WindowEvent* ev = BX_NEW(getAllocator(), WindowEvent)(handle);
		ev->_nwh = nwh;
		_queue.push(ev);
	}

	void PostSuspendEvent(WindowHandle handle, Suspend::Enum state)
	{
		SuspendEvent* ev = BX_NEW(getAllocator(), SuspendEvent)(handle);
		ev->_state = state;
		_queue.push(ev);
	}

	void PostDropFileEvent(WindowHandle handle, const bx::FilePath& filePath)
	{
		DropFileEvent* ev = BX_NEW(getAllocator(), DropFileEvent)(handle);
		ev->_filePath = filePath;
		_queue.push(ev);
	}

	const Event* Poll()
	{
		return _queue.pop();
	}

	const Event* Poll(WindowHandle handle)
	{
		if (isValid(handle) )
		{
			Event* ev = _queue.peek();
			if (NULL == ev
			||  ev->_handle.idx != handle.idx)
			{
				return NULL;
			}
		}

		return Poll();
	}

	void Release(const Event* _event) const
	{
		BX_DELETE(getAllocator(), const_cast<Event*>(_event) );
	}

private:
	bx::SpScUnboundedQueueT<Event> _queue;
};

} // namespace Auto3D
