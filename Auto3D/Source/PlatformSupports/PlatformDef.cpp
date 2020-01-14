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

#include "PlatformSupports/PlatformContext.h"
#include "Application.h"

#include <SDL.h>
#include <SDL_syswm.h>

//extern "C" int32_t Auto3D_main(int32_t _argc, char** _argv);

namespace Auto3D
{
	static uint32_t s_debug = BGFX_DEBUG_NONE;
	static uint32_t s_reset = BGFX_RESET_NONE;
	
	static bool s_exit = false;

	static bx::FileReaderI* s_fileReader = NULL;
	static bx::FileWriterI* s_fileWriter = NULL;

	extern bx::AllocatorI* getDefaultAllocator();
	bx::AllocatorI* g_allocator = getDefaultAllocator();

	typedef bx::StringT<&g_allocator> String;

	static String s_currentDir;


	

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
		s_currentDir.set(_dir);
	}

	class FileReader : public bx::FileReader
	{
		typedef bx::FileReader super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _err);
		}
	};

	class FileWriter : public bx::FileWriter
	{
		typedef bx::FileWriter super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _append, _err);
		}
	};


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

	bool setOrToggle(uint32_t& _flags, const char* _name, uint32_t _bit, int _first, int _argc, char const* const* _argv)
	{
		if (0 == bx::strCmp(_argv[_first], _name) )
		{
			int arg = _first+1;
			if (_argc > arg)
			{
				_flags &= ~_bit;

				bool set = false;
				bx::fromString(&set, _argv[arg]);

				_flags |= set ? _bit : 0;
			}
			else
			{
				_flags ^= _bit;
			}

			return true;
		}

		return false;
	}

	int cmdMouseLock(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
	{
		if (1 < _argc)
		{
			bool set = false;
			if (2 < _argc)
			{
				bx::fromString(&set, _argv[1]);
				InputSetMouseLock(set);
			}
			else
			{
				InputSetMouseLock(!InputIsMouseLocked() );
			}

			return bx::kExitSuccess;
		}

		return bx::kExitFailure;
	}

	int cmdGraphics(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
	{
		if (_argc > 1)
		{
			if (setOrToggle(s_reset, "vsync",       BGFX_RESET_VSYNC,              1, _argc, _argv)
			||  setOrToggle(s_reset, "maxaniso",    BGFX_RESET_MAXANISOTROPY,      1, _argc, _argv)
			||  setOrToggle(s_reset, "msaa",        BGFX_RESET_MSAA_X16,           1, _argc, _argv)
			||  setOrToggle(s_reset, "flush",       BGFX_RESET_FLUSH_AFTER_RENDER, 1, _argc, _argv)
			||  setOrToggle(s_reset, "flip",        BGFX_RESET_FLIP_AFTER_RENDER,  1, _argc, _argv)
			||  setOrToggle(s_reset, "hidpi",       BGFX_RESET_HIDPI,              1, _argc, _argv)
			||  setOrToggle(s_reset, "depthclamp",  BGFX_RESET_DEPTH_CLAMP,        1, _argc, _argv)
			   )
			{
				return bx::kExitSuccess;
			}
			else if (setOrToggle(s_debug, "stats",     BGFX_DEBUG_STATS,     1, _argc, _argv)
				 ||  setOrToggle(s_debug, "ifh",       BGFX_DEBUG_IFH,       1, _argc, _argv)
				 ||  setOrToggle(s_debug, "text",      BGFX_DEBUG_TEXT,      1, _argc, _argv)
				 ||  setOrToggle(s_debug, "wireframe", BGFX_DEBUG_WIREFRAME, 1, _argc, _argv)
				 ||  setOrToggle(s_debug, "profiler",  BGFX_DEBUG_PROFILER,  1, _argc, _argv)
				    )
			{
				bgfx::setDebug(s_debug);
				return bx::kExitSuccess;
			}
			else if (0 == bx::strCmp(_argv[1], "screenshot") )
			{
				bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;

				if (_argc > 2)
				{
					bgfx::requestScreenShot(fbh, _argv[2]);
				}
				else
				{
					time_t tt;
					time(&tt);

					char filePath[256];
					bx::snprintf(filePath, sizeof(filePath), "temp/screenshot-%d", tt);
					bgfx::requestScreenShot(fbh, filePath);
				}

				return bx::kExitSuccess;
			}
			else if (0 == bx::strCmp(_argv[1], "fullscreen") )
			{
				WindowHandle window = { 0 };
				FPlatform::ToggleFullscreen(window);
				return bx::kExitSuccess;
			}
		}

		return bx::kExitFailure;
	}

	int cmdExit(CmdContext* /*_context*/, void* /*_userData*/, int /*_argc*/, char const* const* /*_argv*/)
	{
		s_exit = true;
		return bx::kExitSuccess;
	}

	static const InputBinding s_bindings[] =
	{
		{ Auto3D::Key::KeyQ,         Auto3D::Modifier::LeftCtrl,  1, NULL, "exit"                              },
		{ Auto3D::Key::KeyQ,         Auto3D::Modifier::RightCtrl, 1, NULL, "exit"                              },
		{ Auto3D::Key::KeyF,         Auto3D::Modifier::LeftCtrl,  1, NULL, "graphics fullscreen"               },
		{ Auto3D::Key::KeyF,         Auto3D::Modifier::RightCtrl, 1, NULL, "graphics fullscreen"               },
		{ Auto3D::Key::Return,       Auto3D::Modifier::RightAlt,  1, NULL, "graphics fullscreen"               },
		{ Auto3D::Key::F1,           Auto3D::Modifier::None,      1, NULL, "graphics stats"                    },
		{ Auto3D::Key::F1,           Auto3D::Modifier::LeftCtrl,  1, NULL, "graphics ifh"                      },
		{ Auto3D::Key::GamepadStart, Auto3D::Modifier::None,      1, NULL, "graphics stats"                    },
		{ Auto3D::Key::F1,           Auto3D::Modifier::LeftShift, 1, NULL, "graphics stats 0\ngraphics text 0" },
		{ Auto3D::Key::F3,           Auto3D::Modifier::None,      1, NULL, "graphics wireframe"                },
		{ Auto3D::Key::F4,           Auto3D::Modifier::None,      1, NULL, "graphics hmd"                      },
		{ Auto3D::Key::F4,           Auto3D::Modifier::LeftShift, 1, NULL, "graphics hmdrecenter"              },
		{ Auto3D::Key::F4,           Auto3D::Modifier::LeftCtrl,  1, NULL, "graphics hmddbg"                   },
		{ Auto3D::Key::F6,           Auto3D::Modifier::None,      1, NULL, "graphics profiler"                 },
		{ Auto3D::Key::F7,           Auto3D::Modifier::None,      1, NULL, "graphics vsync"                    },
		{ Auto3D::Key::F8,           Auto3D::Modifier::None,      1, NULL, "graphics msaa"                     },
		{ Auto3D::Key::F9,           Auto3D::Modifier::None,      1, NULL, "graphics flush"                    },
		{ Auto3D::Key::F10,          Auto3D::Modifier::None,      1, NULL, "graphics hidpi"                    },
		{ Auto3D::Key::Print,        Auto3D::Modifier::None,      1, NULL, "graphics screenshot"               },
		{ Auto3D::Key::KeyP,         Auto3D::Modifier::LeftCtrl,  1, NULL, "graphics screenshot"               },

		INPUT_BINDING_END
	};

#if BX_PLATFORM_EMSCRIPTEN
	static AppI* s_app;
	static void updateApp()
	{
		s_app->update();
	}
#endif // BX_PLATFORM_EMSCRIPTEN



	static IAppInstance* getCurrentApp(IAppInstance* _set = NULL)
	{
		if (NULL != _set)
		{
			IAppInstance::s_currentApp = _set;
		}
		else if (NULL == IAppInstance::s_currentApp)
		{
			IAppInstance::s_currentApp = IAppInstance::getFirstApp();
		}

		return IAppInstance::s_currentApp;
	}

	static IAppInstance* getNextWrap(IAppInstance* _app)
	{
		IAppInstance* next = _app->getNext();
		if (NULL != next)
		{
			return next;
		}

		return IAppInstance::getFirstApp();
	}

	int cmdApp(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
	{
		if (0 == bx::strCmp(_argv[1], "restart") )
		{
			if (2 == _argc)
			{
				bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), getCurrentApp()->getName() );
				return bx::kExitSuccess;
			}

			if (0 == bx::strCmp(_argv[2], "next") )
			{
				IAppInstance* next = getNextWrap(getCurrentApp() );
				bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), next->getName() );
				return bx::kExitSuccess;
			}
			else if (0 == bx::strCmp(_argv[2], "prev") )
			{
				IAppInstance* prev = getCurrentApp();
				for (IAppInstance* app = getNextWrap(prev); app != getCurrentApp(); app = getNextWrap(app) )
				{
					prev = app;
				}

				bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), prev->getName() );
				return bx::kExitSuccess;
			}

			for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext() )
			{
				if (0 == bx::strCmp(_argv[2], app->getName() ) )
				{
					bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), app->getName() );
					return bx::kExitSuccess;
				}
			}
		}

		return bx::kExitFailure;
	}

	static int32_t sortApp(const void* _lhs, const void* _rhs)
	{
		const IAppInstance* lhs = *(const IAppInstance**)_lhs;
		const IAppInstance* rhs = *(const IAppInstance**)_rhs;

		return bx::strCmpI(lhs->getName(), rhs->getName() );
	}

	static void sortApps()
	{
		if (2 > IAppInstance::s_numApps)
		{
			return;
		}

		IAppInstance** apps = (IAppInstance**)BX_ALLOC(g_allocator, IAppInstance::s_numApps*sizeof(IAppInstance*) );

		uint32_t ii = 0;
		for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext() )
		{
			apps[ii++] = app;
		}
		bx::quickSort(apps, IAppInstance::s_numApps, sizeof(IAppInstance*), sortApp);

		IAppInstance::s_apps = apps[0];
		for (ii = 1; ii < IAppInstance::s_numApps; ++ii)
		{
			IAppInstance* app = apps[ii-1];
			app->m_next = apps[ii];
		}
		apps[IAppInstance::s_numApps-1]->m_next = NULL;

		BX_FREE(g_allocator, apps);
	}

	int RunMain(int argc, const char* const* argv)
	{
		//DBG(BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME);

		s_fileReader = BX_NEW(g_allocator, FileReader);
		s_fileWriter = BX_NEW(g_allocator, FileWriter);

		CmdInit();
		CmdAdd("mouselock", cmdMouseLock);
		CmdAdd("graphics",  cmdGraphics );
		CmdAdd("exit",      cmdExit     );
		CmdAdd("app",       cmdApp      );

		InputInit();
		InputAddBindings("bindings", s_bindings);

		bx::FilePath fp(argv[0]);
		char title[bx::kMaxFilePath];
		bx::strCopy(title, BX_COUNTOF(title), fp.getBaseName() );

		FPlatform::SetWindowTitle(PlatfromContext::_defaultWindow, title);
		FPlatform::SetWindowSize(PlatfromContext::_defaultWindow, ENTRY_DEFAULT_WIDTH, ENTRY_DEFAULT_HEIGHT);

		sortApps();

		const char* find = "";
		if (1 < argc)
		{
			find = argv[argc-1];
		}

restart:
		IAppInstance* selected = NULL;

		for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext() )
		{
			if (NULL == selected
			&&  !bx::strFindI(app->getName(), find).isEmpty() )
			{
				selected = app;
			}
#if 0
			DBG("%c %s, %s"
				, app == selected ? '>' : ' '
				, app->getName()
				, app->getDescription()
				);
#endif // 0
		}

		int32_t result = bx::kExitSuccess;
		IAppInstance::s_restartArgs[0] = '\0';
		if (0 == IAppInstance::s_numApps)
		{
			result = ::Auto3D_main(argc, (char**)argv);
		}
		else
		{
			result = runApp(getCurrentApp(selected), argc, argv);
		}

		if (0 != bx::strLen(IAppInstance::s_restartArgs) )
		{
			find = IAppInstance::s_restartArgs;
			goto restart;
		}

		FPlatform::SetCurrentDir("");

		InputRemoveBindings("bindings");
		InputShutdown();

		CmdShutdown();

		BX_DELETE(g_allocator, s_fileReader);
		s_fileReader = NULL;

		BX_DELETE(g_allocator, s_fileWriter);
		s_fileWriter = NULL;

		return result;
	}

	WindowState s_window[ENTRY_CONFIG_MAX_WINDOWS];

	bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _debug, uint32_t& _reset, MouseState* _mouse)
	{
		s_debug = _debug;
		s_reset = _reset;

		WindowHandle handle = { UINT16_MAX };

		bool mouseLock = InputIsMouseLocked();

		const Event* ev;
		do
		{
			struct SE { const Event* m_ev; SE() : m_ev(Poll() ) {} ~SE() { if (NULL != m_ev) { Release(m_ev); } } } scopeEvent;
			ev = scopeEvent.m_ev;

			if (NULL != ev)
			{
				switch (ev->_type)
				{
				case Event::Axis:
					{
						const AxisEvent* axis = static_cast<const AxisEvent*>(ev);
						InputSetGamepadAxis(axis->_gamepad, axis->_axis, axis->_value);
					}
					break;

				case Event::Char:
					{
						const CharEvent* chev = static_cast<const CharEvent*>(ev);
						InputChar(chev->_len, chev->_char);
					}
					break;

				case Event::Exit:
					return true;

				case Event::Gamepad:
					{
//						const GamepadEvent* gev = static_cast<const GamepadEvent*>(ev);
//						DBG("gamepad %d, %d", gev->m_gamepad.idx, gev->m_connected);
					}
					break;

				case Event::Mouse:
					{
						const MouseEvent* mouse = static_cast<const MouseEvent*>(ev);
						handle = mouse->_handle;

						InputSetMousePos(mouse->_mx, mouse->_my, mouse->_mz);
						if (!mouse->_move)
						{
							InputSetMouseButtonState(mouse->_button, mouse->_down);
						}

						if (NULL != _mouse
						&&  !mouseLock)
						{
							_mouse->_mx = mouse->_mx;
							_mouse->_my = mouse->_my;
							_mouse->_mz = mouse->_mz;
							if (!mouse->_move)
							{
								_mouse->_buttons[mouse->_button] = mouse->_down;
							}
						}
					}
					break;

				case Event::Key:
					{
						const KeyEvent* key = static_cast<const KeyEvent*>(ev);
						handle = key->_handle;

						InputSetKeyState(key->_key, key->_modifiers, key->_down);
					}
					break;

				case Event::Size:
					{
						const SizeEvent* size = static_cast<const SizeEvent*>(ev);
						WindowState& win = s_window[0];
						win._handle = size->_handle;
						win._width  = size->_width;
						win._height = size->_height;

						handle  = size->_handle;
						_width  = size->_width;
						_height = size->_height;
						_reset  = !s_reset; // force reset
					}
					break;

				case Event::Window:
					break;

				case Event::Suspend:
					break;

				case Event::DropFile:
					{
						const DropFileEvent* drop = static_cast<const DropFileEvent*>(ev);
						DBG("%s", drop->_filePath.getCPtr() );
					}
					break;

				default:
					break;
				}
			}

			InputProcess();

		} while (NULL != ev);

		if (handle.idx == 0
		&&  _reset != s_reset)
		{
			_reset = s_reset;
			bgfx::reset(_width, _height, _reset);
			InputSetMouseResolution(uint16_t(_width), uint16_t(_height) );
		}

		_debug = s_debug;

		IAppInstance::s_width = _width;
		IAppInstance::s_height = _height;

		return s_exit;
	}

	bool processWindowEvents(WindowState& _state, uint32_t& _debug, uint32_t& _reset)
	{
		s_debug = _debug;
		s_reset = _reset;

		WindowHandle handle = { UINT16_MAX };

		bool mouseLock = InputIsMouseLocked();
		bool clearDropFile = true;

		const Event* ev;
		do
		{
			struct SE
			{
				SE(WindowHandle _handle)
					: m_ev(Poll(_handle) )
				{
				}

				~SE()
				{
					if (NULL != m_ev)
					{
						Release(m_ev);
					}
				}

				const Event* m_ev;

			} scopeEvent(handle);
			ev = scopeEvent.m_ev;

			if (NULL != ev)
			{
				handle = ev->_handle;
				WindowState& win = s_window[handle.idx];

				switch (ev->_type)
				{
				case Event::Axis:
					{
						const AxisEvent* axis = static_cast<const AxisEvent*>(ev);
						InputSetGamepadAxis(axis->_gamepad, axis->_axis, axis->_value);
					}
					break;

				case Event::Char:
					{
						const CharEvent* chev = static_cast<const CharEvent*>(ev);
						win._handle = chev->_handle;
						InputChar(chev->_len, chev->_char);
					}
					break;

				case Event::Exit:
					return true;

				case Event::Gamepad:
					{
						const GamepadEvent* gev = static_cast<const GamepadEvent*>(ev);
						DBG("gamepad %d, %d", gev->_gamepad.idx, gev->_connected);
					}
					break;

				case Event::Mouse:
					{
						const MouseEvent* mouse = static_cast<const MouseEvent*>(ev);
						win._handle = mouse->_handle;

						if (mouse->_move)
						{
							InputSetMousePos(mouse->_mx, mouse->_my, mouse->_mz);
						}
						else
						{
							InputSetMouseButtonState(mouse->_button, mouse->_down);
						}

						if (!mouseLock)
						{
							if (mouse->_move)
							{
								win._mouse._mx = mouse->_mx;
								win._mouse._my = mouse->_my;
								win._mouse._mz = mouse->_mz;
							}
							else
							{
								win._mouse._buttons[mouse->_button] = mouse->_down;
							}
						}
					}
					break;

				case Event::Key:
					{
						const KeyEvent* key = static_cast<const KeyEvent*>(ev);
						win._handle = key->_handle;

						InputSetKeyState(key->_key, key->_modifiers, key->_down);
					}
					break;

				case Event::Size:
					{
						const SizeEvent* size = static_cast<const SizeEvent*>(ev);
						win._handle = size->_handle;
						win._width  = size->_width;
						win._height = size->_height;
						_reset  = win._handle.idx == 0
								? !s_reset
								: _reset
								; // force reset
					}
					break;

				case Event::Window:
					{
						const WindowEvent* window = static_cast<const WindowEvent*>(ev);
						win._handle = window->_handle;
						win._nwh    = window->_nwh;
						ev = NULL;
					}
					break;

				case Event::Suspend:
					break;

				case Event::DropFile:
					{
						const DropFileEvent* drop = static_cast<const DropFileEvent*>(ev);
						win._dropFile = drop->_filePath;
						clearDropFile = false;
					}
					break;

				default:
					break;
				}
			}

			InputProcess();

		} while (NULL != ev);

		if (isValid(handle) )
		{
			WindowState& win = s_window[handle.idx];
			if (clearDropFile)
			{
				win._dropFile.clear();
			}

			_state = win;

			if (handle.idx == 0)
			{
				InputSetMouseResolution(uint16_t(win._width), uint16_t(win._height) );
			}
		}

		if (_reset != s_reset)
		{
			_reset = s_reset;
			bgfx::reset(s_window[0]._width, s_window[0]._height, _reset);
			InputSetMouseResolution(uint16_t(s_window[0]._width), uint16_t(s_window[0]._height) );
		}

		_debug = s_debug;

		return s_exit;
	}

	bx::FileReaderI* getFileReader()
	{
		return s_fileReader;
	}

	bx::FileWriterI* getFileWriter()
	{
		return s_fileWriter;
	}

	bx::AllocatorI* getAllocator()
	{
		if (NULL == g_allocator)
		{
			g_allocator = getDefaultAllocator();
		}

		return g_allocator;
	}

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
