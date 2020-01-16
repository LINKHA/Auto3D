#include "Application.h"
#include "Platform/PlatformContext.h"
#include "Platform/Args.h"

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bx/allocator.h>

#include "Platform/Cmd.h"
#include "Platform/Dbg.h"
#include "Platform/Input.h"

//#include "Core/ProcessUtils.h"
//#include "Core/ClassRegister.h"

namespace Auto3D
{

extern bx::AllocatorI* getDefaultAllocator();
bx::AllocatorI* g_allocator = getDefaultAllocator();

typedef bx::StringT<&g_allocator> String;

FString FApplication::_currentDir;

IAppInstance* IAppInstance::_currentApp = NULL;
IAppInstance* IAppInstance::_apps = NULL;
uint32_t IAppInstance::_numApps = 0;
char IAppInstance::_restartArgs[1024] = { '\0' };

WindowState s_window[ENTRY_CONFIG_MAX_WINDOWS];


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

static bx::FileReaderI* s_fileReader = NULL;
static bx::FileWriterI* s_fileWriter = NULL;
static uint32_t s_debug = BGFX_DEBUG_NONE;
static uint32_t s_reset = BGFX_RESET_NONE;

static bool s_exit = false;

bool setOrToggle(uint32_t& _flags, const char* _name, uint32_t _bit, int _first, int _argc, char const* const* _argv)
{
	if (0 == bx::strCmp(_argv[_first], _name))
	{
		int arg = _first + 1;
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

bx::AllocatorI* getAllocator()
{
	if (NULL == g_allocator)
	{
		g_allocator = getDefaultAllocator();
	}

	return g_allocator;
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
			InputSetMouseLock(!InputIsMouseLocked());
		}

		return bx::kExitSuccess;
	}

	return bx::kExitFailure;
}

int cmdGraphics(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	if (_argc > 1)
	{
		if (setOrToggle(s_reset, "vsync", BGFX_RESET_VSYNC, 1, _argc, _argv)
			|| setOrToggle(s_reset, "maxaniso", BGFX_RESET_MAXANISOTROPY, 1, _argc, _argv)
			|| setOrToggle(s_reset, "msaa", BGFX_RESET_MSAA_X16, 1, _argc, _argv)
			|| setOrToggle(s_reset, "flush", BGFX_RESET_FLUSH_AFTER_RENDER, 1, _argc, _argv)
			|| setOrToggle(s_reset, "flip", BGFX_RESET_FLIP_AFTER_RENDER, 1, _argc, _argv)
			|| setOrToggle(s_reset, "hidpi", BGFX_RESET_HIDPI, 1, _argc, _argv)
			|| setOrToggle(s_reset, "depthclamp", BGFX_RESET_DEPTH_CLAMP, 1, _argc, _argv)
			)
		{
			return bx::kExitSuccess;
		}
		else if (setOrToggle(s_debug, "stats", BGFX_DEBUG_STATS, 1, _argc, _argv)
			|| setOrToggle(s_debug, "ifh", BGFX_DEBUG_IFH, 1, _argc, _argv)
			|| setOrToggle(s_debug, "text", BGFX_DEBUG_TEXT, 1, _argc, _argv)
			|| setOrToggle(s_debug, "wireframe", BGFX_DEBUG_WIREFRAME, 1, _argc, _argv)
			|| setOrToggle(s_debug, "profiler", BGFX_DEBUG_PROFILER, 1, _argc, _argv)
			)
		{
			bgfx::setDebug(s_debug);
			return bx::kExitSuccess;
		}
		else if (0 == bx::strCmp(_argv[1], "screenshot"))
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
		else if (0 == bx::strCmp(_argv[1], "fullscreen"))
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

int cmdApp(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	if (0 == bx::strCmp(_argv[1], "restart"))
	{
		if (2 == _argc)
		{
			bx::strCopy(IAppInstance::_restartArgs, BX_COUNTOF(IAppInstance::_restartArgs), IAppInstance::getCurrentApp()->getName());
			return bx::kExitSuccess;
		}

		if (0 == bx::strCmp(_argv[2], "next"))
		{
			IAppInstance* next = IAppInstance::getNextWrap(IAppInstance::getCurrentApp());
			bx::strCopy(IAppInstance::_restartArgs, BX_COUNTOF(IAppInstance::_restartArgs), next->getName());
			return bx::kExitSuccess;
		}
		else if (0 == bx::strCmp(_argv[2], "prev"))
		{
			IAppInstance* prev = IAppInstance::getCurrentApp();
			for (IAppInstance* app = IAppInstance::getNextWrap(prev); app != IAppInstance::getCurrentApp(); app = IAppInstance::getNextWrap(app))
			{
				prev = app;
			}

			bx::strCopy(IAppInstance::_restartArgs, BX_COUNTOF(IAppInstance::_restartArgs), prev->getName());
			return bx::kExitSuccess;
		}

		for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext())
		{
			if (0 == bx::strCmp(_argv[2], app->getName()))
			{
				bx::strCopy(IAppInstance::_restartArgs, BX_COUNTOF(IAppInstance::_restartArgs), app->getName());
				return bx::kExitSuccess;
			}
		}
	}

	return bx::kExitFailure;
}

class FileReader : public bx::FileReader
{
	typedef bx::FileReader super;

public:
	virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
	{
		String filePath(FApplication::_currentDir.CString());
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
		String filePath(FApplication::_currentDir.CString());
		filePath.append(_filePath);
		return super::open(filePath.getPtr(), _append, _err);
	}
};


IAppInstance* IAppInstance::getNextWrap(IAppInstance* app)
{
	IAppInstance* next = app->getNext();
	if (NULL != next)
	{
		return next;
	}

	return getFirstApp();
}

IAppInstance* IAppInstance::getCurrentApp(IAppInstance* set)
{
	if (NULL != set)
	{
		_currentApp = set;
	}
	else if (NULL == _currentApp)
	{
		_currentApp = getFirstApp();
	}

	return _currentApp;
}

static int32_t sortApp(const void* _lhs, const void* _rhs)
{
	const IAppInstance* lhs = *(const IAppInstance**)_lhs;
	const IAppInstance* rhs = *(const IAppInstance**)_rhs;

	return bx::strCmpI(lhs->getName(), rhs->getName());
}

bool processWindowEvents(WindowState& state, uint32_t& debug, uint32_t& reset)
{
	s_debug = debug;
	s_reset = reset;

	WindowHandle handle = { UINT16_MAX };

	bool mouseLock = InputIsMouseLocked();
	bool clearDropFile = true;

	const Event* ev;
	do
	{
		struct SE
		{
			SE(WindowHandle _handle)
				: m_ev(Poll(_handle))
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
				win._width = size->_width;
				win._height = size->_height;
				reset = win._handle.idx == 0
					? !s_reset
					: reset
					; // force reset
			}
			break;

			case Event::Window:
			{
				const WindowEvent* window = static_cast<const WindowEvent*>(ev);
				win._handle = window->_handle;
				win._nwh = window->_nwh;
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

	if (isValid(handle))
	{
		WindowState& win = s_window[handle.idx];
		if (clearDropFile)
		{
			win._dropFile.clear();
		}

		state = win;

		if (handle.idx == 0)
		{
			InputSetMouseResolution(uint16_t(win._width), uint16_t(win._height));
		}
	}

	if (reset != s_reset)
	{
		reset = s_reset;
		bgfx::reset(s_window[0]._width, s_window[0]._height, reset);
		InputSetMouseResolution(uint16_t(s_window[0]._width), uint16_t(s_window[0]._height));
	}

	debug = s_debug;

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


bool processEvents(uint32_t& width, uint32_t& height, uint32_t& debug, uint32_t& reset, MouseState* mouse)
{
	s_debug = debug;
	s_reset = reset;

	WindowHandle handle = { UINT16_MAX };

	bool mouseLock = InputIsMouseLocked();

	const Event* ev;
	do
	{
		struct SE { const Event* m_ev; SE() : m_ev(Poll()) {} ~SE() { if (NULL != m_ev) { Release(m_ev); } } } scopeEvent;
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
				const MouseEvent* mouseEvent = static_cast<const MouseEvent*>(ev);
				handle = mouseEvent->_handle;

				InputSetMousePos(mouseEvent->_mx, mouseEvent->_my, mouseEvent->_mz);
				if (!mouseEvent->_move)
				{
					InputSetMouseButtonState(mouseEvent->_button, mouseEvent->_down);
				}

				if (NULL != mouseEvent
					&& !mouseLock)
				{
					mouse->_mx = mouseEvent->_mx;
					mouse->_my = mouseEvent->_my;
					mouse->_mz = mouseEvent->_mz;
					if (!mouseEvent->_move)
					{
						mouse->_buttons[mouseEvent->_button] = mouseEvent->_down;
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
				win._width = size->_width;
				win._height = size->_height;

				handle = size->_handle;
				width = size->_width;
				height = size->_height;
				reset = !s_reset; // force reset
			}
			break;

			case Event::Window:
				break;

			case Event::Suspend:
				break;

			case Event::DropFile:
			{
				const DropFileEvent* drop = static_cast<const DropFileEvent*>(ev);
				DBG("%s", drop->_filePath.getCPtr());
			}
			break;

			default:
				break;
			}
		}

		InputProcess();

	} while (NULL != ev);

	if (handle.idx == 0
		&& reset != s_reset)
	{
		reset = s_reset;
		bgfx::reset(width, height, reset);
		InputSetMouseResolution(uint16_t(width), uint16_t(height));
	}

	debug = s_debug;

	return s_exit;
}

IAppInstance::IAppInstance(const char* name, const char* description, const char* url)
{
	_name = name;
	_description = description;
	_url = url;
	_next = _apps;

	_apps = this;
	_numApps++;
}

IAppInstance::~IAppInstance()
{
	for (IAppInstance* prev = NULL, *app = _apps, *next = app->getNext()
		; NULL != app
		; prev = app, app = next, next = app->getNext())
	{
		if (app == this)
		{
			if (NULL != prev)
			{
				prev->_next = next;
			}
			else
			{
				_apps = next;
			}

			--_numApps;

			break;
		}
	}
}

void IAppInstance::sortApps()
{
	if (2 > IAppInstance::_numApps)
	{
		return;
	}

	IAppInstance** apps = (IAppInstance**)BX_ALLOC(g_allocator, IAppInstance::_numApps * sizeof(IAppInstance*));

	uint32_t ii = 0;
	for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext())
	{
		apps[ii++] = app;
	}
	bx::quickSort(apps, IAppInstance::_numApps, sizeof(IAppInstance*), sortApp);

	IAppInstance::_apps = apps[0];
	for (ii = 1; ii < IAppInstance::_numApps; ++ii)
	{
		IAppInstance* app = apps[ii - 1];
		app->_next = apps[ii];
	}
	apps[IAppInstance::_numApps - 1]->_next = NULL;

	BX_FREE(g_allocator, apps);
}

IMPLEMENT_SINGLETON(FApplication)

FApplication::FApplication() :
	_exitCode(EXIT_SUCCESS)
{
	_engine = std::make_unique<FEngine>();
}

FApplication::~FApplication()
{
}

int FApplication::Run()
{
	//FClassRegister();
#if !defined(__GNUC__) || __EXCEPTIONS
	try
	{
#endif
		FArgs& args = FArgs::Get();

		PlatfromContext& platfromContext = PlatfromContext::Get();
		platfromContext.Init();

		{
			// Main threa the main operations are implemented here.
			std::thread mainThread(&FApplication::RunMainThread, this);
			mainThread.detach();

			if (!platfromContext.Run())
				ErrorExit();
		}

		// Make sure the main thread is released before you do this.
		platfromContext.DestoryContext();

		return _exitCode;
#if !defined(__GNUC__) || __EXCEPTIONS
	}
	catch (std::bad_alloc&)
	{
		//ErrorDialog("Error", "An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
#endif
}

void FApplication::ErrorExit(const FString& message)
{
	// Close the rendering window
	//_engine->Exit();
	_exitCode = EXIT_FAILURE;

	if (!message.Length())
	{
		//ErrorDialog("Error", _startupErrors.Length() ? _startupErrors :
			//"Application has been terminated due to unexpected error.");
	}
	else {}
		//ErrorDialog("Error", message);
}

int FApplication::RunAppInstance(IAppInstance* app, int argc, const char* const* argv)
{
	// Make sure the engine is created properly
	if (!_engine.get())
		_engine = std::make_unique<FEngine>();

	_engine->Init();
	app->init(AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);


	bgfx::frame();

	FPlatform::SetWindowSize(PlatfromContext::_defaultWindow, AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);

#if BX_PLATFORM_EMSCRIPTEN
	s_app = _app;
	emscripten_set_main_loop(&updateApp, -1, 1);
#else
	for (;;)
	{
		if (_engine->Update())
		{
			if (!app->update())
				break;
			_engine->Render();
			_engine->FrameFinish();
		}

		if (0 != bx::strLen(IAppInstance::_restartArgs))
		{
			break;
		}
	}
#endif // BX_PLATFORM_EMSCRIPTEN
	app->shutdown();
	_engine->Exit();

	return 1;
}

int FApplication::RunMainThread()
{
	FArgs& args = FArgs::Get();
	//DBG(BX_COMPILER_NAME " / " BX_CPU_NAME " / " BX_ARCH_NAME " / " BX_PLATFORM_NAME);

	s_fileReader = BX_NEW(g_allocator, FileReader);
	s_fileWriter = BX_NEW(g_allocator, FileWriter);

	CmdInit();
	CmdAdd("mouselock", cmdMouseLock);
	CmdAdd("graphics", cmdGraphics);
	CmdAdd("exit", cmdExit);
	CmdAdd("app", cmdApp);

	InputInit();
	InputAddBindings("bindings", s_bindings);

	bx::FilePath fp(args._argv[0]);
	char title[bx::kMaxFilePath];
	bx::strCopy(title, BX_COUNTOF(title), fp.getBaseName());

	FPlatform::SetWindowTitle(PlatfromContext::_defaultWindow, title);
	FPlatform::SetWindowSize(PlatfromContext::_defaultWindow, AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);

	IAppInstance::sortApps();

	const char* find = "";
	if (1 < args._argc)
	{
		find = args._argv[args._argc - 1];
	}

restart:
	IAppInstance* selected = NULL;

	for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext())
	{
		if (NULL == selected
			&& !bx::strFindI(app->getName(), find).isEmpty())
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
	IAppInstance::_restartArgs[0] = '\0';
	if (0 == IAppInstance::_numApps)
	{
		// Application instance creates the location.
		result = ::Auto3D_main(args._argc, (char**)args._argv);
	}
	else
	{
		result = RunAppInstance(IAppInstance::getCurrentApp(selected), args._argc, args._argv);
	}

	if (0 != bx::strLen(IAppInstance::_restartArgs))
	{
		find = IAppInstance::_restartArgs;
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


	SDL_Event event;
	SDL_QuitEvent& qev = event.quit;
	qev.type = SDL_QUIT;
	SDL_PushEvent(&event);

	return result;
}

}

int main(int argc, char** argv)
{
	using namespace Auto3D;
	FArgs::Get().Init(argc, argv);

	return FApplication::Get().Run();
}