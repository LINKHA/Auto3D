#pragma once
#include "AutoConfig.h"
#include "Platform/PlatformDef.h"
#include "Platform/GamePad.h"
#include "Container/Singleton.h"
#include "Container/String.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <bx/handlealloc.h>
#include <bx/readerwriter.h>

#include <thread>

#include <bx/os.h>
#include <bx/mutex.h>
#include <bx/thread.h>

#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#	undef None
#endif // defined(None)


#include <tinystl/allocator.h>
#include <tinystl/string.h>

namespace Auto3D
{

struct PlatformMsg
{
	PlatformMsg()
		: _x(0)
		, _y(0)
		, _width(0)
		, _height(0)
		, _flags(0)
		, _flagsEnabled(false)
	{
	}

	int32_t  _x;
	int32_t  _y;
	uint32_t _width;
	uint32_t _height;
	uint32_t _flags;
	FString _title;
	bool _flagsEnabled;
};

struct PlatfromContext
{
	REGISTER_SINGLETON(PlatfromContext)

	PlatfromContext();

	void Init();

	int Run();

	bool DestoryContext();

	WindowHandle FindHandle(uint32_t _windowId);

	WindowHandle FindHandle(SDL_Window* _window);

	void SetWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height, bool _force = false);

	GamepadHandle FindGamepad(SDL_JoystickID _jid);

private:
	inline void InitTranslateKey(uint16_t sdl, Key::Enum key)
	{
		BX_CHECK(sdl < BX_COUNTOF(_translateKey), "Out of bounds %d.", sdl);
		PlatfromContext::_translateKey[sdl & 0xff] = (uint8_t)key;
	}

	inline void InitTranslateGamepad(uint8_t sdl, Key::Enum button)
	{
		_translateGamepad[sdl] = button;
	}

	inline void InitTranslateGamepadAxis(uint8_t sdl, GamepadAxis::Enum axis)
	{
		_translateGamepadAxis[sdl] = uint8_t(axis);
	}

	bool SDLSetWindow(SDL_Window* window)
	{
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(window, &wmi))
		{
			return false;
		}

		bgfx::PlatformData pd;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
		pd.ndt = wmi.info.wl.display;
#		else
		pd.ndt = wmi.info.x11.display;
#		endif
#	elif BX_PLATFORM_OSX
		pd.ndt = NULL;
#	elif BX_PLATFORM_WINDOWS
		pd.ndt = NULL;
#	elif BX_PLATFORM_STEAMLINK
		pd.ndt = wmi.info.vivante.display;
#	endif // BX_PLATFORM_
		pd.nwh = SDLNativeWindowHandle(window);

		pd.context = NULL;
		pd.backBuffer = NULL;
		pd.backBufferDS = NULL;
		bgfx::setPlatformData(pd);

		return true;
	}

	void* SDLNativeWindowHandle(SDL_Window* _window)
	{
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(_window, &wmi))
		{
			return NULL;
		}

#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
		if (!win_impl)
		{
			int width, height;
			SDL_GetWindowSize(_window, &width, &height);
			struct wl_surface* surface = wmi.info.wl.surface;
			if (!surface)
				return nullptr;
			win_impl = wl_egl_window_create(surface, width, height);
			SDL_SetWindowData(_window, "wl_egl_window", win_impl);
		}
		return (void*)(uintptr_t)win_impl;
#		else
		return (void*)wmi.info.x11.window;
#		endif
#	elif BX_PLATFORM_OSX
		return wmi.info.cocoa.window;
#	elif BX_PLATFORM_WINDOWS
		return wmi.info.win.window;
#	elif BX_PLATFORM_STEAMLINK
		return wmi.info.vivante.window;
#	endif // BX_PLATFORM_
	}

	void SDLDestroyWindow(SDL_Window* _window)
	{
		if (!_window)
			return;
#	if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#		if ENTRY_CONFIG_USE_WAYLAND
		wl_egl_window *win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
		if (win_impl)
		{
			SDL_SetWindowData(_window, "wl_egl_window", nullptr);
			wl_egl_window_destroy(win_impl);
		}
#		endif
#	endif
		SDL_DestroyWindow(_window);
	}

	WindowHandle GetWindowHandle(const SDL_UserEvent& _uev)
	{
		union { void* p; WindowHandle h; } cast;
		cast.p = _uev.data1;
		return cast.h;
	}

	uint8_t TranslateKeyModifiers(uint16_t sdl)
	{
		uint8_t modifiers = 0;
		modifiers |= sdl & KMOD_LALT ? Modifier::LeftAlt : 0;
		modifiers |= sdl & KMOD_RALT ? Modifier::RightAlt : 0;
		modifiers |= sdl & KMOD_LCTRL ? Modifier::LeftCtrl : 0;
		modifiers |= sdl & KMOD_RCTRL ? Modifier::RightCtrl : 0;
		modifiers |= sdl & KMOD_LSHIFT ? Modifier::LeftShift : 0;
		modifiers |= sdl & KMOD_RSHIFT ? Modifier::RightShift : 0;
		modifiers |= sdl & KMOD_LGUI ? Modifier::LeftMeta : 0;
		modifiers |= sdl & KMOD_RGUI ? Modifier::RightMeta : 0;
		return modifiers;
	}

	uint8_t TranslateKeyModifierPress(uint16_t key)
	{
		uint8_t modifier;
		switch (key)
		{
		case SDL_SCANCODE_LALT: { modifier = Modifier::LeftAlt;    } break;
		case SDL_SCANCODE_RALT: { modifier = Modifier::RightAlt;   } break;
		case SDL_SCANCODE_LCTRL: { modifier = Modifier::LeftCtrl;   } break;
		case SDL_SCANCODE_RCTRL: { modifier = Modifier::RightCtrl;  } break;
		case SDL_SCANCODE_LSHIFT: { modifier = Modifier::LeftShift;  } break;
		case SDL_SCANCODE_RSHIFT: { modifier = Modifier::RightShift; } break;
		case SDL_SCANCODE_LGUI: { modifier = Modifier::LeftMeta;   } break;
		case SDL_SCANCODE_RGUI: { modifier = Modifier::RightMeta;  } break;
		default: { modifier = 0;                    } break;
		}

		return modifier;
	}

	Key::Enum TranslateKey(SDL_Scancode sdl)
	{
		return (Key::Enum)_translateKey[sdl & 0xff];
	}

	GamepadAxis::Enum TranslateGamepadAxis(uint8_t sdl)
	{
		return GamepadAxis::Enum(_translateGamepadAxis[sdl]);
	}

	Key::Enum TranslateGamepad(uint8_t _sdl)
	{
		return Key::Enum(_translateGamepad[_sdl]);
	}
public:
	
	EventQueue _eventQueue;
	bx::Mutex _lock;

	bx::HandleAllocT<ENTRY_CONFIG_MAX_WINDOWS> _windowAlloc;
	SDL_Window* _window[ENTRY_CONFIG_MAX_WINDOWS];
	uint32_t _flags[ENTRY_CONFIG_MAX_WINDOWS];

	bx::HandleAllocT<ENTRY_CONFIG_MAX_GAMEPADS> _gamepadAlloc;
	FGamepad _gamepad[ENTRY_CONFIG_MAX_GAMEPADS];

	uint32_t _width;
	uint32_t _height;
	float _aspectRatio;

	int32_t _mx;
	int32_t _my;
	int32_t _mz;
	bool _mouseLock;
	bool _fullscreen;

	
	static WindowHandle _defaultWindow;

	static uint8_t _translateKey[256];
	static uint8_t _translateGamepad[256];
	static uint32_t _userEventStart;
	static uint8_t _translateGamepadAxis[256];
};



}