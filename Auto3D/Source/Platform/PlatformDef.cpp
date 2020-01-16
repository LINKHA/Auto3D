#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bgfx/bgfx.h>

#include <time.h>

#if BX_PLATFORM_EMSCRIPTEN
#	include <emscripten.h>
#endif // BX_PLATFORM_EMSCRIPTEN

#include "PlatformDef.h"
#include "cmd.h"
#include "input.h"
#include "dbg.h"

#include "Platform/PlatformContext.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_syswm.h>

//extern "C" int32_t Auto3D_main(int32_t _argc, char** _argv);

namespace Auto3D
{

	const Event* Poll()
	{
		return PlatfromContext::Get()._eventQueue.Poll();
	}

	const Event* Poll(WindowHandle _handle)
	{
		return PlatfromContext::Get()._eventQueue.Poll(_handle);
	}

	void Release(const Event* _event)
	{
		PlatfromContext::Get()._eventQueue.Release(_event);
	}


	static void sdlPostEvent(SDL_USER_WINDOW _type, WindowHandle _handle, PlatformMsg* _msg = NULL, uint32_t _code = 0)
	{
		SDL_Event event;
		SDL_UserEvent& uev = event.user;
		uev.type = PlatfromContext::_userEventStart + _type;

		union { void* p; WindowHandle h; } cast;
		cast.h = _handle;
		uev.data1 = cast.p;

		uev.data2 = _msg;
		uev.code = _code;
		SDL_PushEvent(&event);
	}


	WindowHandle FPlatform::CreateWindowHandle(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags, const char* _title)
	{
		bx::MutexScope scope(PlatfromContext::Get()._lock);
		WindowHandle handle = { PlatfromContext::Get()._windowAlloc.alloc() };

		if (UINT16_MAX != handle.idx)
		{
			PlatformMsg* msg = new PlatformMsg;
			msg->_x = _x;
			msg->_y = _y;
			msg->_width = _width;
			msg->_height = _height;
			msg->_title = _title;
			msg->_flags = _flags;

			sdlPostEvent(SDL_USER_WINDOW_CREATE, handle, msg);
		}

		return handle;
	}

	void FPlatform::DestroyWindowHandle(WindowHandle _handle)
	{
		if (UINT16_MAX != _handle.idx)
		{
			sdlPostEvent(SDL_USER_WINDOW_DESTROY, _handle);

			bx::MutexScope scope(PlatfromContext::Get()._lock);
			PlatfromContext::Get()._windowAlloc.free(_handle.idx);
		}
	}

	void FPlatform::SetWindowPos(WindowHandle _handle, int32_t _x, int32_t _y)
	{
		PlatformMsg* msg = new PlatformMsg;
		msg->_x = _x;
		msg->_y = _y;

		sdlPostEvent(SDL_USER_WINDOW_SET_POS, _handle, msg);
	}

	void FPlatform::SetWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height)
	{
		PlatformMsg* msg = new PlatformMsg;
		msg->_width = _width;
		msg->_height = _height;

		sdlPostEvent(SDL_USER_WINDOW_SET_SIZE, _handle, msg);
	}

	void FPlatform::SetWindowTitle(WindowHandle _handle, const char* _title)
	{
		PlatformMsg* msg = new PlatformMsg;
		msg->_title = _title;

		sdlPostEvent(SDL_USER_WINDOW_SET_TITLE, _handle, msg);
	}

	void FPlatform::SetWindowFlags(WindowHandle _handle, uint32_t _flags, bool _enabled)
	{
		PlatformMsg* msg = new PlatformMsg;
		msg->_flags = _flags;
		msg->_flagsEnabled = _enabled;
		sdlPostEvent(SDL_USER_WINDOW_SET_FLAGS, _handle, msg);
	}

	void FPlatform::ToggleFullscreen(WindowHandle _handle)
	{
		sdlPostEvent(SDL_USER_WINDOW_TOGGLE_FULL_SCREEN, _handle);
	}

	void FPlatform::SetMouseLock(WindowHandle _handle, bool _lock)
	{
		sdlPostEvent(SDL_USER_WINDOW_MOUSE_LOCK, _handle, NULL, _lock);
	}


	void FPlatform::SetCurrentDir(const char* _dir)
	{
		FApplication::_currentDir = _dir;
	}

#if ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
	bx::AllocatorI* getDefaultAllocator()
	{
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();
	}
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

	static const char* s_keyName[] =
	{
		"None",
		"Esc",
		"Return",
		"Tab",
		"Space",
		"Backspace",
		"Up",
		"Down",
		"Left",
		"Right",
		"Insert",
		"Delete",
		"Home",
		"End",
		"PageUp",
		"PageDown",
		"Print",
		"Plus",
		"Minus",
		"LeftBracket",
		"RightBracket",
		"Semicolon",
		"Quote",
		"Comma",
		"Period",
		"Slash",
		"Backslash",
		"Tilde",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"NumPad0",
		"NumPad1",
		"NumPad2",
		"NumPad3",
		"NumPad4",
		"NumPad5",
		"NumPad6",
		"NumPad7",
		"NumPad8",
		"NumPad9",
		"Key0",
		"Key1",
		"Key2",
		"Key3",
		"Key4",
		"Key5",
		"Key6",
		"Key7",
		"Key8",
		"Key9",
		"KeyA",
		"KeyB",
		"KeyC",
		"KeyD",
		"KeyE",
		"KeyF",
		"KeyG",
		"KeyH",
		"KeyI",
		"KeyJ",
		"KeyK",
		"KeyL",
		"KeyM",
		"KeyN",
		"KeyO",
		"KeyP",
		"KeyQ",
		"KeyR",
		"KeyS",
		"KeyT",
		"KeyU",
		"KeyV",
		"KeyW",
		"KeyX",
		"KeyY",
		"KeyZ",
		"GamepadA",
		"GamepadB",
		"GamepadX",
		"GamepadY",
		"GamepadThumbL",
		"GamepadThumbR",
		"GamepadShoulderL",
		"GamepadShoulderR",
		"GamepadUp",
		"GamepadDown",
		"GamepadLeft",
		"GamepadRight",
		"GamepadBack",
		"GamepadStart",
		"GamepadGuide",
	};
	BX_STATIC_ASSERT(Key::Count == BX_COUNTOF(s_keyName) );

	const char* getName(Key::Enum _key)
	{
		BX_CHECK(_key < Key::Count, "Invalid key %d.", _key);
		return s_keyName[_key];
	}

	char keyToAscii(Key::Enum _key, uint8_t _modifiers)
	{
		const bool isAscii = (Key::Key0 <= _key && _key <= Key::KeyZ)
						  || (Key::Esc  <= _key && _key <= Key::Minus);
		if (!isAscii)
		{
			return '\0';
		}

		const bool isNumber = (Key::Key0 <= _key && _key <= Key::Key9);
		if (isNumber)
		{
			return '0' + char(_key - Key::Key0);
		}

		const bool isChar = (Key::KeyA <= _key && _key <= Key::KeyZ);
		if (isChar)
		{
			enum { ShiftMask = Modifier::LeftShift|Modifier::RightShift };

			const bool shift = !!(_modifiers&ShiftMask);
			return (shift ? 'A' : 'a') + char(_key - Key::KeyA);
		}

		switch (_key)
		{
		case Key::Esc:       return 0x1b;
		case Key::Return:    return '\n';
		case Key::Tab:       return '\t';
		case Key::Space:     return ' ';
		case Key::Backspace: return 0x08;
		case Key::Plus:      return '+';
		case Key::Minus:     return '-';
		default:             break;
		}

		return '\0';
	}

#if BX_PLATFORM_EMSCRIPTEN
	static AppI* s_app;
	static void updateApp()
	{
		s_app->update();
	}
#endif // BX_PLATFORM_EMSCRIPTEN

	void* TinyStlAllocator::static_allocate(size_t _bytes)
	{
		return BX_ALLOC(getAllocator(), _bytes);
	}

	void TinyStlAllocator::static_deallocate(void* _ptr, size_t /*_bytes*/)
	{
		if (NULL != _ptr)
		{
			BX_FREE(getAllocator(), _ptr);
		}
	}


} // namespace Auto3D

extern "C" bool entry_process_events(uint32_t* _width, uint32_t* _height, uint32_t* _debug, uint32_t* _reset)
{
	return Auto3D::processEvents(*_width, *_height, *_debug, *_reset, NULL);
}
