#pragma once
#include "PlatformContext.h"

#include "PlatformSupports/PlatformDef.h"
#include "PlatformSupports/Platform.h"

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#	if ENTRY_CONFIG_USE_WAYLAND
#		include <wayland-egl.h>
#	endif 
#elif BX_PLATFORM_WINDOWS
#	define SDL_MAIN_HANDLED
#endif

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
BX_PRAGMA_DIAGNOSTIC_POP()


namespace Auto3D
{


uint8_t PlatfromContext::_translateKey[256] = { 0 };
uint8_t PlatfromContext::_translateGamepad[256] = { 0 };
uint32_t PlatfromContext::_userEventStart = { 0 };
uint8_t PlatfromContext::_translateGamepadAxis[256] = { 0 };
WindowHandle PlatfromContext::_defaultWindow = { 0 };

IMPLEMENT_SINGLETON(PlatfromContext)

PlatfromContext::PlatfromContext()
	: _width(ENTRY_DEFAULT_WIDTH)
	, _height(ENTRY_DEFAULT_HEIGHT)
	, _aspectRatio(16.0f / 9.0f)
	, _mx(0)
	, _my(0)
	, _mz(0)
	, _mouseLock(false)
	, _fullscreen(false)
{
	bx::memSet(_translateKey, 0, sizeof(_translateKey));
	InitTranslateKey(SDL_SCANCODE_ESCAPE, Key::Esc);
	InitTranslateKey(SDL_SCANCODE_RETURN, Key::Return);
	InitTranslateKey(SDL_SCANCODE_TAB, Key::Tab);
	InitTranslateKey(SDL_SCANCODE_BACKSPACE, Key::Backspace);
	InitTranslateKey(SDL_SCANCODE_SPACE, Key::Space);
	InitTranslateKey(SDL_SCANCODE_UP, Key::Up);
	InitTranslateKey(SDL_SCANCODE_DOWN, Key::Down);
	InitTranslateKey(SDL_SCANCODE_LEFT, Key::Left);
	InitTranslateKey(SDL_SCANCODE_RIGHT, Key::Right);
	InitTranslateKey(SDL_SCANCODE_PAGEUP, Key::PageUp);
	InitTranslateKey(SDL_SCANCODE_PAGEDOWN, Key::PageDown);
	InitTranslateKey(SDL_SCANCODE_HOME, Key::Home);
	InitTranslateKey(SDL_SCANCODE_END, Key::End);
	InitTranslateKey(SDL_SCANCODE_PRINTSCREEN, Key::Print);
	InitTranslateKey(SDL_SCANCODE_KP_PLUS, Key::Plus);
	InitTranslateKey(SDL_SCANCODE_EQUALS, Key::Plus);
	InitTranslateKey(SDL_SCANCODE_KP_MINUS, Key::Minus);
	InitTranslateKey(SDL_SCANCODE_MINUS, Key::Minus);
	InitTranslateKey(SDL_SCANCODE_GRAVE, Key::Tilde);
	InitTranslateKey(SDL_SCANCODE_KP_COMMA, Key::Comma);
	InitTranslateKey(SDL_SCANCODE_COMMA, Key::Comma);
	InitTranslateKey(SDL_SCANCODE_KP_PERIOD, Key::Period);
	InitTranslateKey(SDL_SCANCODE_PERIOD, Key::Period);
	InitTranslateKey(SDL_SCANCODE_SLASH, Key::Slash);
	InitTranslateKey(SDL_SCANCODE_F1, Key::F1);
	InitTranslateKey(SDL_SCANCODE_F2, Key::F2);
	InitTranslateKey(SDL_SCANCODE_F3, Key::F3);
	InitTranslateKey(SDL_SCANCODE_F4, Key::F4);
	InitTranslateKey(SDL_SCANCODE_F5, Key::F5);
	InitTranslateKey(SDL_SCANCODE_F6, Key::F6);
	InitTranslateKey(SDL_SCANCODE_F7, Key::F7);
	InitTranslateKey(SDL_SCANCODE_F8, Key::F8);
	InitTranslateKey(SDL_SCANCODE_F9, Key::F9);
	InitTranslateKey(SDL_SCANCODE_F10, Key::F10);
	InitTranslateKey(SDL_SCANCODE_F11, Key::F11);
	InitTranslateKey(SDL_SCANCODE_F12, Key::F12);
	InitTranslateKey(SDL_SCANCODE_KP_0, Key::NumPad0);
	InitTranslateKey(SDL_SCANCODE_KP_1, Key::NumPad1);
	InitTranslateKey(SDL_SCANCODE_KP_2, Key::NumPad2);
	InitTranslateKey(SDL_SCANCODE_KP_3, Key::NumPad3);
	InitTranslateKey(SDL_SCANCODE_KP_4, Key::NumPad4);
	InitTranslateKey(SDL_SCANCODE_KP_5, Key::NumPad5);
	InitTranslateKey(SDL_SCANCODE_KP_6, Key::NumPad6);
	InitTranslateKey(SDL_SCANCODE_KP_7, Key::NumPad7);
	InitTranslateKey(SDL_SCANCODE_KP_8, Key::NumPad8);
	InitTranslateKey(SDL_SCANCODE_KP_9, Key::NumPad9);
	InitTranslateKey(SDL_SCANCODE_0, Key::Key0);
	InitTranslateKey(SDL_SCANCODE_1, Key::Key1);
	InitTranslateKey(SDL_SCANCODE_2, Key::Key2);
	InitTranslateKey(SDL_SCANCODE_3, Key::Key3);
	InitTranslateKey(SDL_SCANCODE_4, Key::Key4);
	InitTranslateKey(SDL_SCANCODE_5, Key::Key5);
	InitTranslateKey(SDL_SCANCODE_6, Key::Key6);
	InitTranslateKey(SDL_SCANCODE_7, Key::Key7);
	InitTranslateKey(SDL_SCANCODE_8, Key::Key8);
	InitTranslateKey(SDL_SCANCODE_9, Key::Key9);
	InitTranslateKey(SDL_SCANCODE_A, Key::KeyA);
	InitTranslateKey(SDL_SCANCODE_B, Key::KeyB);
	InitTranslateKey(SDL_SCANCODE_C, Key::KeyC);
	InitTranslateKey(SDL_SCANCODE_D, Key::KeyD);
	InitTranslateKey(SDL_SCANCODE_E, Key::KeyE);
	InitTranslateKey(SDL_SCANCODE_F, Key::KeyF);
	InitTranslateKey(SDL_SCANCODE_G, Key::KeyG);
	InitTranslateKey(SDL_SCANCODE_H, Key::KeyH);
	InitTranslateKey(SDL_SCANCODE_I, Key::KeyI);
	InitTranslateKey(SDL_SCANCODE_J, Key::KeyJ);
	InitTranslateKey(SDL_SCANCODE_K, Key::KeyK);
	InitTranslateKey(SDL_SCANCODE_L, Key::KeyL);
	InitTranslateKey(SDL_SCANCODE_M, Key::KeyM);
	InitTranslateKey(SDL_SCANCODE_N, Key::KeyN);
	InitTranslateKey(SDL_SCANCODE_O, Key::KeyO);
	InitTranslateKey(SDL_SCANCODE_P, Key::KeyP);
	InitTranslateKey(SDL_SCANCODE_Q, Key::KeyQ);
	InitTranslateKey(SDL_SCANCODE_R, Key::KeyR);
	InitTranslateKey(SDL_SCANCODE_S, Key::KeyS);
	InitTranslateKey(SDL_SCANCODE_T, Key::KeyT);
	InitTranslateKey(SDL_SCANCODE_U, Key::KeyU);
	InitTranslateKey(SDL_SCANCODE_V, Key::KeyV);
	InitTranslateKey(SDL_SCANCODE_W, Key::KeyW);
	InitTranslateKey(SDL_SCANCODE_X, Key::KeyX);
	InitTranslateKey(SDL_SCANCODE_Y, Key::KeyY);
	InitTranslateKey(SDL_SCANCODE_Z, Key::KeyZ);

	bx::memSet(_translateGamepad, uint8_t(Key::Count), sizeof(_translateGamepad));
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_A, Key::GamepadA);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_B, Key::GamepadB);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_X, Key::GamepadX);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_Y, Key::GamepadY);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSTICK, Key::GamepadThumbL);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSTICK, Key::GamepadThumbR);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, Key::GamepadShoulderL);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Key::GamepadShoulderR);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_UP, Key::GamepadUp);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_DOWN, Key::GamepadDown);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_LEFT, Key::GamepadLeft);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, Key::GamepadRight);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_BACK, Key::GamepadBack);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_START, Key::GamepadStart);
	InitTranslateGamepad(SDL_CONTROLLER_BUTTON_GUIDE, Key::GamepadGuide);

	bx::memSet(_translateGamepadAxis, uint8_t(GamepadAxis::Count), sizeof(_translateGamepadAxis));
	InitTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTX, GamepadAxis::LeftX);
	InitTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTY, GamepadAxis::LeftY);
	InitTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, GamepadAxis::LeftZ);
	InitTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTX, GamepadAxis::RightX);
	InitTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTY, GamepadAxis::RightY);
	InitTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, GamepadAxis::RightZ);
}

void PlatfromContext::Init(int _argc, char** _argv)
{

	SDL_Init(0
		| SDL_INIT_GAMECONTROLLER
	);

	_windowAlloc.alloc();
	_window[0] = SDL_CreateWindow("bgfx"
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, _width
		, _height
		, SDL_WINDOW_SHOWN
		| SDL_WINDOW_RESIZABLE
	);

	_flags[0] = 0
		| ENTRY_WINDOW_FLAG_ASPECT_RATIO
		| ENTRY_WINDOW_FLAG_FRAME
		;

	_userEventStart = SDL_RegisterEvents(7);

	SDLSetWindow(_window[0]);
	bgfx::renderFrame();

}

int PlatfromContext::Run(int _argc, char** _argv)
{
	// Force window resolution...
	
	SetWindowSize(_defaultWindow, _width, _height, true);

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

	bx::FileReaderI* reader = NULL;
	while (NULL == reader)
	{
		reader = getFileReader();
		bx::sleep(100);
	}

	if (bx::open(reader, "gamecontrollerdb.txt"))
	{
		bx::AllocatorI* allocator = getAllocator();
		uint32_t size = (uint32_t)bx::getSize(reader);
		void* data = BX_ALLOC(allocator, size + 1);
		bx::read(reader, data, size);
		bx::close(reader);
		((char*)data)[size] = '\0';

		if (SDL_GameControllerAddMapping((char*)data) < 0) {
			DBG("SDL game controller add mapping failed: %s", SDL_GetError());
		}

		BX_FREE(allocator, data);
	}

	bool exit = false;
	SDL_Event event;
	while (!exit)
	{
		bgfx::renderFrame();

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				_eventQueue.postExitEvent();
				exit = true;
				break;

			case SDL_MOUSEMOTION:
			{
				const SDL_MouseMotionEvent& mev = event.motion;
				_mx = mev.x;
				_my = mev.y;

				WindowHandle handle = FindHandle(mev.windowID);
				if (isValid(handle))
				{
					_eventQueue.postMouseEvent(handle, _mx, _my, _mz);
				}
			}
			break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				const SDL_MouseButtonEvent& mev = event.button;
				WindowHandle handle = FindHandle(mev.windowID);
				if (isValid(handle))
				{
					MouseButton::Enum button;
					switch (mev.button)
					{
					default:
					case SDL_BUTTON_LEFT:   button = MouseButton::Left;   break;
					case SDL_BUTTON_MIDDLE: button = MouseButton::Middle; break;
					case SDL_BUTTON_RIGHT:  button = MouseButton::Right;  break;
					}

					_eventQueue.postMouseEvent(handle
						, mev.x
						, mev.y
						, _mz
						, button
						, mev.type == SDL_MOUSEBUTTONDOWN
					);
				}
			}
			break;

			case SDL_MOUSEWHEEL:
			{
				const SDL_MouseWheelEvent& mev = event.wheel;
				_mz += mev.y;

				WindowHandle handle = FindHandle(mev.windowID);
				if (isValid(handle))
				{
					_eventQueue.postMouseEvent(handle, _mx, _my, _mz);
				}
			}
			break;

			case SDL_TEXTINPUT:
			{
				const SDL_TextInputEvent& tev = event.text;
				WindowHandle handle = FindHandle(tev.windowID);
				if (isValid(handle))
				{
					_eventQueue.postCharEvent(handle, 1, (const uint8_t*)tev.text);
				}
			}
			break;

			case SDL_KEYDOWN:
			{
				const SDL_KeyboardEvent& kev = event.key;
				WindowHandle handle = FindHandle(kev.windowID);
				if (isValid(handle))
				{
					uint8_t modifiers = TranslateKeyModifiers(kev.keysym.mod);
					Key::Enum key = TranslateKey(kev.keysym.scancode);

#if 0
					DBG("SDL scancode %d, key %d, name %s, key name %s"
						, kev.keysym.scancode
						, key
						, SDL_GetScancodeName(kev.keysym.scancode)
						, SDL_GetKeyName(kev.keysym.scancode)
					);
#endif // 0

					/// If you only press (e.g.) 'shift' and nothing else, then key == 'shift', modifier == 0.
					/// Further along, pressing 'shift' + 'ctrl' would be: key == 'shift', modifier == 'ctrl.
					if (0 == key && 0 == modifiers)
					{
						modifiers = TranslateKeyModifierPress(kev.keysym.scancode);
					}

					if (Key::Esc == key)
					{
						uint8_t pressedChar[4];
						pressedChar[0] = 0x1b;
						_eventQueue.postCharEvent(handle, 1, pressedChar);
					}
					else if (Key::Return == key)
					{
						uint8_t pressedChar[4];
						pressedChar[0] = 0x0d;
						_eventQueue.postCharEvent(handle, 1, pressedChar);
					}
					else if (Key::Backspace == key)
					{
						uint8_t pressedChar[4];
						pressedChar[0] = 0x08;
						_eventQueue.postCharEvent(handle, 1, pressedChar);
					}

					_eventQueue.postKeyEvent(handle, key, modifiers, kev.state == SDL_PRESSED);
				}
			}
			break;

			case SDL_KEYUP:
			{
				const SDL_KeyboardEvent& kev = event.key;
				WindowHandle handle = FindHandle(kev.windowID);
				if (isValid(handle))
				{
					uint8_t modifiers = TranslateKeyModifiers(kev.keysym.mod);
					Key::Enum key = TranslateKey(kev.keysym.scancode);
					_eventQueue.postKeyEvent(handle, key, modifiers, kev.state == SDL_PRESSED);
				}
			}
			break;

			case SDL_WINDOWEVENT:
			{
				const SDL_WindowEvent& wev = event.window;
				switch (wev.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				{
					WindowHandle handle = FindHandle(wev.windowID);
					SetWindowSize(handle, wev.data1, wev.data2);
				}
				break;

				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_EXPOSED:
				case SDL_WINDOWEVENT_MOVED:
				case SDL_WINDOWEVENT_MINIMIZED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				case SDL_WINDOWEVENT_RESTORED:
				case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				case SDL_WINDOWEVENT_FOCUS_LOST:
					break;

				case SDL_WINDOWEVENT_CLOSE:
				{
					WindowHandle handle = FindHandle(wev.windowID);
					if (0 == handle.idx)
					{
						_eventQueue.postExitEvent();
						exit = true;
					}
				}
				break;
				}
			}
			break;

			case SDL_JOYAXISMOTION:
			{
				const SDL_JoyAxisEvent& jev = event.jaxis;
				GamepadHandle handle = FindGamepad(jev.which);
				if (isValid(handle))
				{
					GamepadAxis::Enum axis = TranslateGamepadAxis(jev.axis);
					_gamepad[handle.idx].update(_eventQueue, _defaultWindow, handle, axis, jev.value);
				}
			}
			break;

			case SDL_CONTROLLERAXISMOTION:
			{
				const SDL_ControllerAxisEvent& aev = event.caxis;
				GamepadHandle handle = FindGamepad(aev.which);
				if (isValid(handle))
				{
					GamepadAxis::Enum axis = TranslateGamepadAxis(aev.axis);
					_gamepad[handle.idx].update(_eventQueue, _defaultWindow, handle, axis, aev.value);
				}
			}
			break;

			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:
			{
				const SDL_JoyButtonEvent& bev = event.jbutton;
				GamepadHandle handle = FindGamepad(bev.which);

				if (isValid(handle))
				{
					Key::Enum key = TranslateGamepad(bev.button);
					if (Key::Count != key)
					{
						_eventQueue.postKeyEvent(_defaultWindow, key, 0, event.type == SDL_JOYBUTTONDOWN);
					}
				}
			}
			break;

			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
			{
				const SDL_ControllerButtonEvent& bev = event.cbutton;
				GamepadHandle handle = FindGamepad(bev.which);
				if (isValid(handle))
				{
					Key::Enum key = TranslateGamepad(bev.button);
					if (Key::Count != key)
					{
						_eventQueue.postKeyEvent(_defaultWindow, key, 0, event.type == SDL_CONTROLLERBUTTONDOWN);
					}
				}
			}
			break;

			case SDL_JOYDEVICEADDED:
			{
				GamepadHandle handle = { _gamepadAlloc.alloc() };
				if (isValid(handle))
				{
					const SDL_JoyDeviceEvent& jev = event.jdevice;
					_gamepad[handle.idx].create(jev);
					_eventQueue.postGamepadEvent(_defaultWindow, handle, true);
				}
			}
			break;

			case SDL_JOYDEVICEREMOVED:
			{
				const SDL_JoyDeviceEvent& jev = event.jdevice;
				GamepadHandle handle = FindGamepad(jev.which);
				if (isValid(handle))
				{
					_gamepad[handle.idx].destroy();
					_gamepadAlloc.free(handle.idx);
					_eventQueue.postGamepadEvent(_defaultWindow, handle, false);
				}
			}
			break;

			case SDL_CONTROLLERDEVICEADDED:
			{
				GamepadHandle handle = { _gamepadAlloc.alloc() };
				if (isValid(handle))
				{
					const SDL_ControllerDeviceEvent& cev = event.cdevice;
					_gamepad[handle.idx].create(cev);
					_eventQueue.postGamepadEvent(_defaultWindow, handle, true);
				}
			}
			break;

			case SDL_CONTROLLERDEVICEREMAPPED:
			{

			}
			break;

			case SDL_CONTROLLERDEVICEREMOVED:
			{
				const SDL_ControllerDeviceEvent& cev = event.cdevice;
				GamepadHandle handle = FindGamepad(cev.which);
				if (isValid(handle))
				{
					_gamepad[handle.idx].destroy();
					_gamepadAlloc.free(handle.idx);
					_eventQueue.postGamepadEvent(_defaultWindow, handle, false);
				}
			}
			break;

			case SDL_DROPFILE:
			{
				const SDL_DropEvent& dev = event.drop;
				WindowHandle handle = _defaultWindow; //findHandle(dev.windowID);
				if (isValid(handle))
				{
					_eventQueue.postDropFileEvent(handle, dev.file);
					SDL_free(dev.file);
				}
			}
			break;

			default:
			{
				const SDL_UserEvent& uev = event.user;
				switch (uev.type - _userEventStart)
				{
				case SDL_USER_WINDOW_CREATE:
				{
					WindowHandle handle = GetWindowHandle(uev);
					PlatformMsg* msg = (PlatformMsg*)uev.data2;

					_window[handle.idx] = SDL_CreateWindow(msg->_title.CString()
						, msg->_x
						, msg->_y
						, msg->_width
						, msg->_height
						, SDL_WINDOW_SHOWN
						| SDL_WINDOW_RESIZABLE
					);

					_flags[handle.idx] = msg->_flags;

					void* nwh = SDLNativeWindowHandle(_window[handle.idx]);
					if (NULL != nwh)
					{
						_eventQueue.postSizeEvent(handle, msg->_width, msg->_height);
						_eventQueue.postWindowEvent(handle, nwh);
					}

					delete msg;
				}
				break;

				case SDL_USER_WINDOW_DESTROY:
				{
					WindowHandle handle = GetWindowHandle(uev);
					if (isValid(handle))
					{
						_eventQueue.postWindowEvent(handle);
						SDLDestroyWindow(_window[handle.idx]);
						_window[handle.idx] = NULL;
					}
				}
				break;

				case SDL_USER_WINDOW_SET_TITLE:
				{
					WindowHandle handle = GetWindowHandle(uev);
					PlatformMsg* msg = (PlatformMsg*)uev.data2;
					if (isValid(handle))
					{
						SDL_SetWindowTitle(_window[handle.idx], msg->_title.CString());
					}
					delete msg;
				}
				break;

				case SDL_USER_WINDOW_SET_FLAGS:
				{
					WindowHandle handle = GetWindowHandle(uev);
					PlatformMsg* msg = (PlatformMsg*)uev.data2;

					if (msg->_flagsEnabled)
					{
						_flags[handle.idx] |= msg->_flags;
					}
					else
					{
						_flags[handle.idx] &= ~msg->_flags;
					}

					delete msg;
				}
				break;

				case SDL_USER_WINDOW_SET_POS:
				{
					WindowHandle handle = GetWindowHandle(uev);
					PlatformMsg* msg = (PlatformMsg*)uev.data2;
					SDL_SetWindowPosition(_window[handle.idx], msg->_x, msg->_y);
					delete msg;
				}
				break;

				case SDL_USER_WINDOW_SET_SIZE:
				{
					WindowHandle handle = GetWindowHandle(uev);
					PlatformMsg* msg = (PlatformMsg*)uev.data2;
					if (isValid(handle))
					{
						SetWindowSize(handle, msg->_width, msg->_height);
					}
					delete msg;
				}
				break;

				case SDL_USER_WINDOW_TOGGLE_FRAME:
				{
					WindowHandle handle = GetWindowHandle(uev);
					if (isValid(handle))
					{
						_flags[handle.idx] ^= ENTRY_WINDOW_FLAG_FRAME;
						SDL_SetWindowBordered(_window[handle.idx], (SDL_bool)!!(_flags[handle.idx] & ENTRY_WINDOW_FLAG_FRAME));
					}
				}
				break;

				case SDL_USER_WINDOW_TOGGLE_FULL_SCREEN:
				{
					WindowHandle handle = GetWindowHandle(uev);
					_fullscreen = !_fullscreen;
					SDL_SetWindowFullscreen(_window[handle.idx], _fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				}
				break;

				case SDL_USER_WINDOW_MOUSE_LOCK:
				{
					SDL_SetRelativeMouseMode(!!uev.code ? SDL_TRUE : SDL_FALSE);
				}
				break;

				default:
					break;
				}
			}
			break;
			}
		}
	}

	while (bgfx::RenderFrame::NoContext != bgfx::renderFrame()) {};


	return true;
}

bool PlatfromContext::DestoryContext()
{
	SDLDestroyWindow(_window[0]);
	SDL_Quit();

	return true;
}
WindowHandle PlatfromContext::FindHandle(uint32_t _windowId)
{
	SDL_Window* window = SDL_GetWindowFromID(_windowId);
	return FindHandle(window);
}

WindowHandle PlatfromContext::FindHandle(SDL_Window* window)
{
	bx::MutexScope scope(_lock);
	for (uint32_t ii = 0, num = _windowAlloc.getNumHandles(); ii < num; ++ii)
	{
		uint16_t idx = _windowAlloc.getHandleAt(ii);
		if (window == _window[idx])
		{
			WindowHandle handle = { idx };
			return handle;
		}
	}

	WindowHandle invalid = { UINT16_MAX };
	return invalid;
}

void PlatfromContext::SetWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height, bool _force)
{
	if (_width != _width
		|| _height != _height
		|| _force)
	{
		_width = _width;
		_height = _height;

		SDL_SetWindowSize(_window[_handle.idx], _width, _height);
		_eventQueue.postSizeEvent(_handle, _width, _height);
	}
}

GamepadHandle PlatfromContext::FindGamepad(SDL_JoystickID _jid)
{
	for (uint32_t ii = 0, num = _gamepadAlloc.getNumHandles(); ii < num; ++ii)
	{
		uint16_t idx = _gamepadAlloc.getHandleAt(ii);
		if (_jid == _gamepad[idx].m_jid)
		{
			GamepadHandle handle = { idx };
			return handle;
		}
	}

	GamepadHandle invalid = { UINT16_MAX };
	return invalid;
}
}