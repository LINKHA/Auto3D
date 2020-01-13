#pragma once
#include "AutoConfig.h"
#include "PlatformSupports/PlatformDef.h"
#include "PlatformSupports/GamePad.h"
#include "Container/Singleton.h"
#include "Container/String.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <bx/mutex.h>
#include <bx/thread.h>
#include <bx/handlealloc.h>
#include <bx/readerwriter.h>

#include <thread>

#include <bx/os.h>

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

struct FMainThreadEntry
{
	int _argc;
	char** _argv;

	static int32_t ThreadFunc(bx::Thread* thread, void* userData);
};

struct PlatfromContext
{
	REGISTER_SINGLETON(PlatfromContext)
	PlatfromContext();

	int Run(int _argc, char** _argv);

	WindowHandle FindHandle(uint32_t _windowId);

	WindowHandle FindHandle(SDL_Window* _window);

	void SetWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height, bool _force = false);

	GamepadHandle FindGamepad(SDL_JoystickID _jid);




	inline void initTranslateKey(uint16_t _sdl, Key::Enum _key)
	{
		BX_CHECK(_sdl < BX_COUNTOF(s_translateKey), "Out of bounds %d.", _sdl);
		PlatfromContext::s_translateKey[_sdl & 0xff] = (uint8_t)_key;
	}

	inline void initTranslateGamepad(uint8_t _sdl, Key::Enum _button)
	{
		s_translateGamepad[_sdl] = _button;
	}

	inline void initTranslateGamepadAxis(uint8_t _sdl, GamepadAxis::Enum _axis)
	{
		s_translateGamepadAxis[_sdl] = uint8_t(_axis);
	}

	bool sdlSetWindow(SDL_Window* _window)
	{
		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);
		if (!SDL_GetWindowWMInfo(_window, &wmi))
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
		pd.nwh = SDLNativeWindowHandle(_window);

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

	static uint8_t translateKeyModifiers(uint16_t _sdl)
	{
		uint8_t modifiers = 0;
		modifiers |= _sdl & KMOD_LALT ? Modifier::LeftAlt : 0;
		modifiers |= _sdl & KMOD_RALT ? Modifier::RightAlt : 0;
		modifiers |= _sdl & KMOD_LCTRL ? Modifier::LeftCtrl : 0;
		modifiers |= _sdl & KMOD_RCTRL ? Modifier::RightCtrl : 0;
		modifiers |= _sdl & KMOD_LSHIFT ? Modifier::LeftShift : 0;
		modifiers |= _sdl & KMOD_RSHIFT ? Modifier::RightShift : 0;
		modifiers |= _sdl & KMOD_LGUI ? Modifier::LeftMeta : 0;
		modifiers |= _sdl & KMOD_RGUI ? Modifier::RightMeta : 0;
		return modifiers;
	}

	static uint8_t translateKeyModifierPress(uint16_t _key)
	{
		uint8_t modifier;
		switch (_key)
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

	static Key::Enum translateKey(SDL_Scancode _sdl)
	{
		return (Key::Enum)s_translateKey[_sdl & 0xff];
	}

	static GamepadAxis::Enum translateGamepadAxis(uint8_t _sdl)
	{
		return GamepadAxis::Enum(s_translateGamepadAxis[_sdl]);
	}

	static Key::Enum translateGamepad(uint8_t _sdl)
	{
		return Key::Enum(s_translateGamepad[_sdl]);
	}

	static WindowHandle getWindowHandle(const SDL_UserEvent& _uev)
	{
		union { void* p; WindowHandle h; } cast;
		cast.p = _uev.data1;
		return cast.h;
	}

	static void sdlDestroyWindow(SDL_Window* _window)
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

	static WindowHandle _defaultWindow;

	FMainThreadEntry m_mte;
	bx::Thread m_thread;

	EventQueue m_eventQueue;
	bx::Mutex m_lock;

	bx::HandleAllocT<ENTRY_CONFIG_MAX_WINDOWS> _windowAlloc;
	SDL_Window* m_window[ENTRY_CONFIG_MAX_WINDOWS];
	uint32_t m_flags[ENTRY_CONFIG_MAX_WINDOWS];

	bx::HandleAllocT<ENTRY_CONFIG_MAX_GAMEPADS> m_gamepadAlloc;
	FGamepad m_gamepad[ENTRY_CONFIG_MAX_GAMEPADS];

	uint32_t m_width;
	uint32_t m_height;
	float m_aspectRatio;

	int32_t m_mx;
	int32_t m_my;
	int32_t m_mz;
	bool m_mouseLock;
	bool m_fullscreen;

	

	static uint8_t s_translateKey[256];
	static uint8_t s_translateGamepad[256];
	static uint32_t s_userEventStart;
	static uint8_t s_translateGamepadAxis[256];
};


}