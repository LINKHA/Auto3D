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

IAppInstance*    IAppInstance::s_currentApp = NULL;
IAppInstance*    IAppInstance::s_apps = NULL;
uint32_t IAppInstance::s_numApps = 0;
char IAppInstance::s_restartArgs[1024] = { '\0' };

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

int RunAppInstance(IAppInstance* app, int argc, const char* const* argv)
{
	app->init(AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);
	bgfx::frame();

	FPlatform::SetWindowSize(PlatfromContext::_defaultWindow, AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);

#if BX_PLATFORM_EMSCRIPTEN
	s_app = _app;
	emscripten_set_main_loop(&updateApp, -1, 1);
#else
	while (app->update())
	{
		if (0 != bx::strLen(IAppInstance::s_restartArgs))
		{
			break;
		}
	}
#endif // BX_PLATFORM_EMSCRIPTEN

	return app->shutdown();
}


static bx::FileReaderI* s_fileReader = NULL;
static bx::FileWriterI* s_fileWriter = NULL;
static uint32_t s_debug = BGFX_DEBUG_NONE;
static uint32_t s_reset = BGFX_RESET_NONE;

static bool s_exit = false;

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




IAppInstance* FApplication::getCurrentApp(IAppInstance* set)
{
	if (NULL != set)
	{
		IAppInstance::s_currentApp = set;
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

bx::AllocatorI* getAllocator()
{
	if (NULL == g_allocator)
	{
		g_allocator = getDefaultAllocator();
	}

	return g_allocator;
}

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
			bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs),FApplication::getCurrentApp()->getName());
			return bx::kExitSuccess;
		}

		if (0 == bx::strCmp(_argv[2], "next"))
		{
			IAppInstance* next = getNextWrap(FApplication::getCurrentApp());
			bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), next->getName());
			return bx::kExitSuccess;
		}
		else if (0 == bx::strCmp(_argv[2], "prev"))
		{
			IAppInstance* prev = FApplication::getCurrentApp();
			for (IAppInstance* app = getNextWrap(prev); app != FApplication::getCurrentApp(); app = getNextWrap(app))
			{
				prev = app;
			}

			bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), prev->getName());
			return bx::kExitSuccess;
		}

		for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext())
		{
			if (0 == bx::strCmp(_argv[2], app->getName()))
			{
				bx::strCopy(IAppInstance::s_restartArgs, BX_COUNTOF(IAppInstance::s_restartArgs), app->getName());
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

	return bx::strCmpI(lhs->getName(), rhs->getName());
}

static void sortApps()
{
	if (2 > IAppInstance::s_numApps)
	{
		return;
	}

	IAppInstance** apps = (IAppInstance**)BX_ALLOC(g_allocator, IAppInstance::s_numApps * sizeof(IAppInstance*));

	uint32_t ii = 0;
	for (IAppInstance* app = IAppInstance::getFirstApp(); NULL != app; app = app->getNext())
	{
		apps[ii++] = app;
	}
	bx::quickSort(apps, IAppInstance::s_numApps, sizeof(IAppInstance*), sortApp);

	IAppInstance::s_apps = apps[0];
	for (ii = 1; ii < IAppInstance::s_numApps; ++ii)
	{
		IAppInstance* app = apps[ii - 1];
		app->m_next = apps[ii];
	}
	apps[IAppInstance::s_numApps - 1]->m_next = NULL;

	BX_FREE(g_allocator, apps);
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
				_reset = win._handle.idx == 0
					? !s_reset
					: _reset
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

		_state = win;

		if (handle.idx == 0)
		{
			InputSetMouseResolution(uint16_t(win._width), uint16_t(win._height));
		}
	}

	if (_reset != s_reset)
	{
		_reset = s_reset;
		bgfx::reset(s_window[0]._width, s_window[0]._height, _reset);
		InputSetMouseResolution(uint16_t(s_window[0]._width), uint16_t(s_window[0]._height));
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


int RunMain(int argc, const char* const* argv)
{
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

	bx::FilePath fp(argv[0]);
	char title[bx::kMaxFilePath];
	bx::strCopy(title, BX_COUNTOF(title), fp.getBaseName());

	FPlatform::SetWindowTitle(PlatfromContext::_defaultWindow, title);
	FPlatform::SetWindowSize(PlatfromContext::_defaultWindow, AUTO_DEFAULT_WIDTH, AUTO_DEFAULT_HEIGHT);

	sortApps();

	const char* find = "";
	if (1 < argc)
	{
		find = argv[argc - 1];
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
	IAppInstance::s_restartArgs[0] = '\0';
	if (0 == IAppInstance::s_numApps)
	{
		result = ::Auto3D_main(argc, (char**)argv);
	}
	else
	{
		result = RunAppInstance(FApplication::getCurrentApp(selected), argc, argv);
	}

	if (0 != bx::strLen(IAppInstance::s_restartArgs))
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

bool processEvents(uint32_t& _width, uint32_t& _height, uint32_t& _debug, uint32_t& _reset, MouseState* _mouse)
{
	s_debug = _debug;
	s_reset = _reset;

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
				const MouseEvent* mouse = static_cast<const MouseEvent*>(ev);
				handle = mouse->_handle;

				InputSetMousePos(mouse->_mx, mouse->_my, mouse->_mz);
				if (!mouse->_move)
				{
					InputSetMouseButtonState(mouse->_button, mouse->_down);
				}

				if (NULL != _mouse
					&& !mouseLock)
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
				win._width = size->_width;
				win._height = size->_height;

				handle = size->_handle;
				_width = size->_width;
				_height = size->_height;
				_reset = !s_reset; // force reset
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
		&& _reset != s_reset)
	{
		_reset = s_reset;
		bgfx::reset(_width, _height, _reset);
		InputSetMouseResolution(uint16_t(_width), uint16_t(_height));
	}

	_debug = s_debug;

	return s_exit;
}

IAppInstance::IAppInstance(const char* _name, const char* _description, const char* _url)
{
	m_name = _name;
	m_description = _description;
	m_url = _url;
	m_next = s_apps;

	s_apps = this;
	s_numApps++;
}

IAppInstance::~IAppInstance()
{
	for (IAppInstance* prev = NULL, *app = s_apps, *next = app->getNext()
		; NULL != app
		; prev = app, app = next, next = app->getNext())
	{
		if (app == this)
		{
			if (NULL != prev)
			{
				prev->m_next = next;
			}
			else
			{
				s_apps = next;
			}

			--s_numApps;

			break;
		}
	}
}

const char* IAppInstance::getName() const
{
	return m_name;
}

const char* IAppInstance::getDescription() const
{
	return m_description;
}

const char* IAppInstance::getUrl() const
{
	return m_url;
}

IAppInstance* IAppInstance::getNext()
{
	return m_next;
}

FApplication::FApplication() :
	_exitCode(EXIT_SUCCESS)
{

}

FApplication::~FApplication()
{
}

int FApplication::Run()
{
	// Make sure the engine is created properly
	/*if (_engine.Null())
		_engine = MakeUnique<AEngine>();
*/
	//FClassRegister();
#if !defined(__GNUC__) || __EXCEPTIONS
	try
	{
#endif
		PlatfromContext& platfromContext = PlatfromContext::Get();

		FArgs& args = FArgs::Get();

		platfromContext.Init();

		_mainThread.init(FApplication::MainThreadFunc);

		if (!platfromContext.Run())
		{
			ErrorExit();
		}

		_mainThread.shutdown();
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

int32_t FApplication::MainThreadFunc(bx::Thread* thread,void* userData)
{
	BX_UNUSED(thread);
	FArgs& args = FArgs::Get();
	int32_t result = RunMain(args._argc, args._argv);

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

	FApplication app;
	return app.Run();
}