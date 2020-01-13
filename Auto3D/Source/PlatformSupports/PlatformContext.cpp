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

int32_t FMainThreadEntry::threadFunc(bx::Thread* thread, void* userData)
{
	BX_UNUSED(thread);

	FMainThreadEntry* self = (FMainThreadEntry*)userData;
	int32_t result = RunMain(self->m_argc, self->m_argv);

	SDL_Event event;
	SDL_QuitEvent& qev = event.quit;
	qev.type = SDL_QUIT;
	SDL_PushEvent(&event);
	return result;
}

const Event* poll()
{
	return PlatfromContext::Get().m_eventQueue.poll();
}

const Event* poll(WindowHandle _handle)
{
	return PlatfromContext::Get().m_eventQueue.poll(_handle);
}

void release(const Event* _event)
{
	PlatfromContext::Get().m_eventQueue.release(_event);
}

uint8_t PlatfromContext::s_translateKey[256] = { 0 };
uint8_t PlatfromContext::s_translateGamepad[256] = { 0 };
uint32_t PlatfromContext::s_userEventStart = { 0 };
uint8_t PlatfromContext::s_translateGamepadAxis[256] = { 0 };

IMPLEMENT_SINGLETON(PlatfromContext)

PlatfromContext::PlatfromContext()
	: m_width(ENTRY_DEFAULT_WIDTH)
	, m_height(ENTRY_DEFAULT_HEIGHT)
	, m_aspectRatio(16.0f / 9.0f)
	, m_mx(0)
	, m_my(0)
	, m_mz(0)
	, m_mouseLock(false)
	, m_fullscreen(false)
{
	bx::memSet(s_translateKey, 0, sizeof(s_translateKey));
	initTranslateKey(SDL_SCANCODE_ESCAPE, Key::Esc);
	initTranslateKey(SDL_SCANCODE_RETURN, Key::Return);
	initTranslateKey(SDL_SCANCODE_TAB, Key::Tab);
	initTranslateKey(SDL_SCANCODE_BACKSPACE, Key::Backspace);
	initTranslateKey(SDL_SCANCODE_SPACE, Key::Space);
	initTranslateKey(SDL_SCANCODE_UP, Key::Up);
	initTranslateKey(SDL_SCANCODE_DOWN, Key::Down);
	initTranslateKey(SDL_SCANCODE_LEFT, Key::Left);
	initTranslateKey(SDL_SCANCODE_RIGHT, Key::Right);
	initTranslateKey(SDL_SCANCODE_PAGEUP, Key::PageUp);
	initTranslateKey(SDL_SCANCODE_PAGEDOWN, Key::PageDown);
	initTranslateKey(SDL_SCANCODE_HOME, Key::Home);
	initTranslateKey(SDL_SCANCODE_END, Key::End);
	initTranslateKey(SDL_SCANCODE_PRINTSCREEN, Key::Print);
	initTranslateKey(SDL_SCANCODE_KP_PLUS, Key::Plus);
	initTranslateKey(SDL_SCANCODE_EQUALS, Key::Plus);
	initTranslateKey(SDL_SCANCODE_KP_MINUS, Key::Minus);
	initTranslateKey(SDL_SCANCODE_MINUS, Key::Minus);
	initTranslateKey(SDL_SCANCODE_GRAVE, Key::Tilde);
	initTranslateKey(SDL_SCANCODE_KP_COMMA, Key::Comma);
	initTranslateKey(SDL_SCANCODE_COMMA, Key::Comma);
	initTranslateKey(SDL_SCANCODE_KP_PERIOD, Key::Period);
	initTranslateKey(SDL_SCANCODE_PERIOD, Key::Period);
	initTranslateKey(SDL_SCANCODE_SLASH, Key::Slash);
	initTranslateKey(SDL_SCANCODE_F1, Key::F1);
	initTranslateKey(SDL_SCANCODE_F2, Key::F2);
	initTranslateKey(SDL_SCANCODE_F3, Key::F3);
	initTranslateKey(SDL_SCANCODE_F4, Key::F4);
	initTranslateKey(SDL_SCANCODE_F5, Key::F5);
	initTranslateKey(SDL_SCANCODE_F6, Key::F6);
	initTranslateKey(SDL_SCANCODE_F7, Key::F7);
	initTranslateKey(SDL_SCANCODE_F8, Key::F8);
	initTranslateKey(SDL_SCANCODE_F9, Key::F9);
	initTranslateKey(SDL_SCANCODE_F10, Key::F10);
	initTranslateKey(SDL_SCANCODE_F11, Key::F11);
	initTranslateKey(SDL_SCANCODE_F12, Key::F12);
	initTranslateKey(SDL_SCANCODE_KP_0, Key::NumPad0);
	initTranslateKey(SDL_SCANCODE_KP_1, Key::NumPad1);
	initTranslateKey(SDL_SCANCODE_KP_2, Key::NumPad2);
	initTranslateKey(SDL_SCANCODE_KP_3, Key::NumPad3);
	initTranslateKey(SDL_SCANCODE_KP_4, Key::NumPad4);
	initTranslateKey(SDL_SCANCODE_KP_5, Key::NumPad5);
	initTranslateKey(SDL_SCANCODE_KP_6, Key::NumPad6);
	initTranslateKey(SDL_SCANCODE_KP_7, Key::NumPad7);
	initTranslateKey(SDL_SCANCODE_KP_8, Key::NumPad8);
	initTranslateKey(SDL_SCANCODE_KP_9, Key::NumPad9);
	initTranslateKey(SDL_SCANCODE_0, Key::Key0);
	initTranslateKey(SDL_SCANCODE_1, Key::Key1);
	initTranslateKey(SDL_SCANCODE_2, Key::Key2);
	initTranslateKey(SDL_SCANCODE_3, Key::Key3);
	initTranslateKey(SDL_SCANCODE_4, Key::Key4);
	initTranslateKey(SDL_SCANCODE_5, Key::Key5);
	initTranslateKey(SDL_SCANCODE_6, Key::Key6);
	initTranslateKey(SDL_SCANCODE_7, Key::Key7);
	initTranslateKey(SDL_SCANCODE_8, Key::Key8);
	initTranslateKey(SDL_SCANCODE_9, Key::Key9);
	initTranslateKey(SDL_SCANCODE_A, Key::KeyA);
	initTranslateKey(SDL_SCANCODE_B, Key::KeyB);
	initTranslateKey(SDL_SCANCODE_C, Key::KeyC);
	initTranslateKey(SDL_SCANCODE_D, Key::KeyD);
	initTranslateKey(SDL_SCANCODE_E, Key::KeyE);
	initTranslateKey(SDL_SCANCODE_F, Key::KeyF);
	initTranslateKey(SDL_SCANCODE_G, Key::KeyG);
	initTranslateKey(SDL_SCANCODE_H, Key::KeyH);
	initTranslateKey(SDL_SCANCODE_I, Key::KeyI);
	initTranslateKey(SDL_SCANCODE_J, Key::KeyJ);
	initTranslateKey(SDL_SCANCODE_K, Key::KeyK);
	initTranslateKey(SDL_SCANCODE_L, Key::KeyL);
	initTranslateKey(SDL_SCANCODE_M, Key::KeyM);
	initTranslateKey(SDL_SCANCODE_N, Key::KeyN);
	initTranslateKey(SDL_SCANCODE_O, Key::KeyO);
	initTranslateKey(SDL_SCANCODE_P, Key::KeyP);
	initTranslateKey(SDL_SCANCODE_Q, Key::KeyQ);
	initTranslateKey(SDL_SCANCODE_R, Key::KeyR);
	initTranslateKey(SDL_SCANCODE_S, Key::KeyS);
	initTranslateKey(SDL_SCANCODE_T, Key::KeyT);
	initTranslateKey(SDL_SCANCODE_U, Key::KeyU);
	initTranslateKey(SDL_SCANCODE_V, Key::KeyV);
	initTranslateKey(SDL_SCANCODE_W, Key::KeyW);
	initTranslateKey(SDL_SCANCODE_X, Key::KeyX);
	initTranslateKey(SDL_SCANCODE_Y, Key::KeyY);
	initTranslateKey(SDL_SCANCODE_Z, Key::KeyZ);

	bx::memSet(s_translateGamepad, uint8_t(Key::Count), sizeof(s_translateGamepad));
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_A, Key::GamepadA);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_B, Key::GamepadB);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_X, Key::GamepadX);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_Y, Key::GamepadY);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSTICK, Key::GamepadThumbL);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSTICK, Key::GamepadThumbR);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_LEFTSHOULDER, Key::GamepadShoulderL);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, Key::GamepadShoulderR);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_UP, Key::GamepadUp);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_DOWN, Key::GamepadDown);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_LEFT, Key::GamepadLeft);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, Key::GamepadRight);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_BACK, Key::GamepadBack);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_START, Key::GamepadStart);
	initTranslateGamepad(SDL_CONTROLLER_BUTTON_GUIDE, Key::GamepadGuide);

	bx::memSet(s_translateGamepadAxis, uint8_t(GamepadAxis::Count), sizeof(s_translateGamepadAxis));
	initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTX, GamepadAxis::LeftX);
	initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_LEFTY, GamepadAxis::LeftY);
	initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERLEFT, GamepadAxis::LeftZ);
	initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTX, GamepadAxis::RightX);
	initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_RIGHTY, GamepadAxis::RightY);
	initTranslateGamepadAxis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT, GamepadAxis::RightZ);
}

int PlatfromContext::Run(int _argc, char** _argv)
{
	m_mte.m_argc = _argc;
	m_mte.m_argv = _argv;

	SDL_Init(0
		| SDL_INIT_GAMECONTROLLER
	);

	_windowAlloc.alloc();
	m_window[0] = SDL_CreateWindow("bgfx"
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, m_width
		, m_height
		, SDL_WINDOW_SHOWN
		| SDL_WINDOW_RESIZABLE
	);

	m_flags[0] = 0
		| ENTRY_WINDOW_FLAG_ASPECT_RATIO
		| ENTRY_WINDOW_FLAG_FRAME
		;

	s_userEventStart = SDL_RegisterEvents(7);

	sdlSetWindow(m_window[0]);
	bgfx::renderFrame();

	m_thread.init(FMainThreadEntry::threadFunc, &m_mte);

	// Force window resolution...
	WindowHandle defaultWindow = { 0 };
	SetWindowSize(defaultWindow, m_width, m_height, true);

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
				m_eventQueue.postExitEvent();
				exit = true;
				break;

			case SDL_MOUSEMOTION:
			{
				const SDL_MouseMotionEvent& mev = event.motion;
				m_mx = mev.x;
				m_my = mev.y;

				WindowHandle handle = FindHandle(mev.windowID);
				if (isValid(handle))
				{
					m_eventQueue.postMouseEvent(handle, m_mx, m_my, m_mz);
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

					m_eventQueue.postMouseEvent(handle
						, mev.x
						, mev.y
						, m_mz
						, button
						, mev.type == SDL_MOUSEBUTTONDOWN
					);
				}
			}
			break;

			case SDL_MOUSEWHEEL:
			{
				const SDL_MouseWheelEvent& mev = event.wheel;
				m_mz += mev.y;

				WindowHandle handle = FindHandle(mev.windowID);
				if (isValid(handle))
				{
					m_eventQueue.postMouseEvent(handle, m_mx, m_my, m_mz);
				}
			}
			break;

			case SDL_TEXTINPUT:
			{
				const SDL_TextInputEvent& tev = event.text;
				WindowHandle handle = FindHandle(tev.windowID);
				if (isValid(handle))
				{
					m_eventQueue.postCharEvent(handle, 1, (const uint8_t*)tev.text);
				}
			}
			break;

			case SDL_KEYDOWN:
			{
				const SDL_KeyboardEvent& kev = event.key;
				WindowHandle handle = FindHandle(kev.windowID);
				if (isValid(handle))
				{
					uint8_t modifiers = translateKeyModifiers(kev.keysym.mod);
					Key::Enum key = translateKey(kev.keysym.scancode);

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
						modifiers = translateKeyModifierPress(kev.keysym.scancode);
					}

					if (Key::Esc == key)
					{
						uint8_t pressedChar[4];
						pressedChar[0] = 0x1b;
						m_eventQueue.postCharEvent(handle, 1, pressedChar);
					}
					else if (Key::Return == key)
					{
						uint8_t pressedChar[4];
						pressedChar[0] = 0x0d;
						m_eventQueue.postCharEvent(handle, 1, pressedChar);
					}
					else if (Key::Backspace == key)
					{
						uint8_t pressedChar[4];
						pressedChar[0] = 0x08;
						m_eventQueue.postCharEvent(handle, 1, pressedChar);
					}

					m_eventQueue.postKeyEvent(handle, key, modifiers, kev.state == SDL_PRESSED);
				}
			}
			break;

			case SDL_KEYUP:
			{
				const SDL_KeyboardEvent& kev = event.key;
				WindowHandle handle = FindHandle(kev.windowID);
				if (isValid(handle))
				{
					uint8_t modifiers = translateKeyModifiers(kev.keysym.mod);
					Key::Enum key = translateKey(kev.keysym.scancode);
					m_eventQueue.postKeyEvent(handle, key, modifiers, kev.state == SDL_PRESSED);
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
						m_eventQueue.postExitEvent();
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
					GamepadAxis::Enum axis = translateGamepadAxis(jev.axis);
					m_gamepad[handle.idx].update(m_eventQueue, defaultWindow, handle, axis, jev.value);
				}
			}
			break;

			case SDL_CONTROLLERAXISMOTION:
			{
				const SDL_ControllerAxisEvent& aev = event.caxis;
				GamepadHandle handle = FindGamepad(aev.which);
				if (isValid(handle))
				{
					GamepadAxis::Enum axis = translateGamepadAxis(aev.axis);
					m_gamepad[handle.idx].update(m_eventQueue, defaultWindow, handle, axis, aev.value);
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
					Key::Enum key = translateGamepad(bev.button);
					if (Key::Count != key)
					{
						m_eventQueue.postKeyEvent(defaultWindow, key, 0, event.type == SDL_JOYBUTTONDOWN);
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
					Key::Enum key = translateGamepad(bev.button);
					if (Key::Count != key)
					{
						m_eventQueue.postKeyEvent(defaultWindow, key, 0, event.type == SDL_CONTROLLERBUTTONDOWN);
					}
				}
			}
			break;

			case SDL_JOYDEVICEADDED:
			{
				GamepadHandle handle = { m_gamepadAlloc.alloc() };
				if (isValid(handle))
				{
					const SDL_JoyDeviceEvent& jev = event.jdevice;
					m_gamepad[handle.idx].create(jev);
					m_eventQueue.postGamepadEvent(defaultWindow, handle, true);
				}
			}
			break;

			case SDL_JOYDEVICEREMOVED:
			{
				const SDL_JoyDeviceEvent& jev = event.jdevice;
				GamepadHandle handle = FindGamepad(jev.which);
				if (isValid(handle))
				{
					m_gamepad[handle.idx].destroy();
					m_gamepadAlloc.free(handle.idx);
					m_eventQueue.postGamepadEvent(defaultWindow, handle, false);
				}
			}
			break;

			case SDL_CONTROLLERDEVICEADDED:
			{
				GamepadHandle handle = { m_gamepadAlloc.alloc() };
				if (isValid(handle))
				{
					const SDL_ControllerDeviceEvent& cev = event.cdevice;
					m_gamepad[handle.idx].create(cev);
					m_eventQueue.postGamepadEvent(defaultWindow, handle, true);
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
					m_gamepad[handle.idx].destroy();
					m_gamepadAlloc.free(handle.idx);
					m_eventQueue.postGamepadEvent(defaultWindow, handle, false);
				}
			}
			break;

			case SDL_DROPFILE:
			{
				const SDL_DropEvent& dev = event.drop;
				WindowHandle handle = defaultWindow; //findHandle(dev.windowID);
				if (isValid(handle))
				{
					m_eventQueue.postDropFileEvent(handle, dev.file);
					SDL_free(dev.file);
				}
			}
			break;

			default:
			{
				const SDL_UserEvent& uev = event.user;
				switch (uev.type - s_userEventStart)
				{
				case SDL_USER_WINDOW_CREATE:
				{
					WindowHandle handle = getWindowHandle(uev);
					Msg* msg = (Msg*)uev.data2;

					m_window[handle.idx] = SDL_CreateWindow(msg->m_title.c_str()
						, msg->m_x
						, msg->m_y
						, msg->m_width
						, msg->m_height
						, SDL_WINDOW_SHOWN
						| SDL_WINDOW_RESIZABLE
					);

					m_flags[handle.idx] = msg->m_flags;

					void* nwh = SDLNativeWindowHandle(m_window[handle.idx]);
					if (NULL != nwh)
					{
						m_eventQueue.postSizeEvent(handle, msg->m_width, msg->m_height);
						m_eventQueue.postWindowEvent(handle, nwh);
					}

					delete msg;
				}
				break;

				case SDL_USER_WINDOW_DESTROY:
				{
					WindowHandle handle = getWindowHandle(uev);
					if (isValid(handle))
					{
						m_eventQueue.postWindowEvent(handle);
						sdlDestroyWindow(m_window[handle.idx]);
						m_window[handle.idx] = NULL;
					}
				}
				break;

				case SDL_USER_WINDOW_SET_TITLE:
				{
					WindowHandle handle = getWindowHandle(uev);
					Msg* msg = (Msg*)uev.data2;
					if (isValid(handle))
					{
						SDL_SetWindowTitle(m_window[handle.idx], msg->m_title.c_str());
					}
					delete msg;
				}
				break;

				case SDL_USER_WINDOW_SET_FLAGS:
				{
					WindowHandle handle = getWindowHandle(uev);
					Msg* msg = (Msg*)uev.data2;

					if (msg->m_flagsEnabled)
					{
						m_flags[handle.idx] |= msg->m_flags;
					}
					else
					{
						m_flags[handle.idx] &= ~msg->m_flags;
					}

					delete msg;
				}
				break;

				case SDL_USER_WINDOW_SET_POS:
				{
					WindowHandle handle = getWindowHandle(uev);
					Msg* msg = (Msg*)uev.data2;
					SDL_SetWindowPosition(m_window[handle.idx], msg->m_x, msg->m_y);
					delete msg;
				}
				break;

				case SDL_USER_WINDOW_SET_SIZE:
				{
					WindowHandle handle = getWindowHandle(uev);
					Msg* msg = (Msg*)uev.data2;
					if (isValid(handle))
					{
						SetWindowSize(handle, msg->m_width, msg->m_height);
					}
					delete msg;
				}
				break;

				case SDL_USER_WINDOW_TOGGLE_FRAME:
				{
					WindowHandle handle = getWindowHandle(uev);
					if (isValid(handle))
					{
						m_flags[handle.idx] ^= ENTRY_WINDOW_FLAG_FRAME;
						SDL_SetWindowBordered(m_window[handle.idx], (SDL_bool)!!(m_flags[handle.idx] & ENTRY_WINDOW_FLAG_FRAME));
					}
				}
				break;

				case SDL_USER_WINDOW_TOGGLE_FULL_SCREEN:
				{
					WindowHandle handle = getWindowHandle(uev);
					m_fullscreen = !m_fullscreen;
					SDL_SetWindowFullscreen(m_window[handle.idx], m_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
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
	m_thread.shutdown();

	sdlDestroyWindow(m_window[0]);
	SDL_Quit();

	return m_thread.getExitCode();
}

WindowHandle PlatfromContext::FindHandle(uint32_t _windowId)
{
	SDL_Window* window = SDL_GetWindowFromID(_windowId);
	return FindHandle(window);
}

WindowHandle PlatfromContext::FindHandle(SDL_Window* _window)
{
	bx::MutexScope scope(m_lock);
	for (uint32_t ii = 0, num = _windowAlloc.getNumHandles(); ii < num; ++ii)
	{
		uint16_t idx = _windowAlloc.getHandleAt(ii);
		if (_window == m_window[idx])
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
	if (_width != m_width
		|| _height != m_height
		|| _force)
	{
		m_width = _width;
		m_height = _height;

		SDL_SetWindowSize(m_window[_handle.idx], m_width, m_height);
		m_eventQueue.postSizeEvent(_handle, m_width, m_height);
	}
}

GamepadHandle PlatfromContext::FindGamepad(SDL_JoystickID _jid)
{
	for (uint32_t ii = 0, num = m_gamepadAlloc.getNumHandles(); ii < num; ++ii)
	{
		uint16_t idx = m_gamepadAlloc.getHandleAt(ii);
		if (_jid == m_gamepad[idx].m_jid)
		{
			GamepadHandle handle = { idx };
			return handle;
		}
	}

	GamepadHandle invalid = { UINT16_MAX };
	return invalid;
}
}