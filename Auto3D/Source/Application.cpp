#include "Application.h"
#include "PlatformSupports/PlatformContext.h"
//#include "Core/ProcessUtils.h"
//#include "Core/ClassRegister.h"

namespace Auto3D
{
uint32_t IAppInstance::s_width = ENTRY_DEFAULT_WIDTH;
uint32_t IAppInstance::s_height = ENTRY_DEFAULT_HEIGHT;

IAppInstance*    IAppInstance::s_currentApp = NULL;
IAppInstance*    IAppInstance::s_apps = NULL;
uint32_t IAppInstance::s_numApps = 0;
char IAppInstance::s_restartArgs[1024] = { '\0' };

int runApp(IAppInstance* app, int argc, const char* const* argv)
{
	app->init(argc, argv, IAppInstance::s_width, IAppInstance::s_height);
	bgfx::frame();

	FPlatform::SetWindowSize(PlatfromContext::_defaultWindow, IAppInstance::s_width, IAppInstance::s_height);

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

int32_t FMainThreadEntry::ThreadFunc(bx::Thread* thread, void* userData)
{
	BX_UNUSED(thread);

	FMainThreadEntry* self = (FMainThreadEntry*)userData;
	int32_t result = RunMain(self->_argc, self->_argv);

	SDL_Event event;
	SDL_QuitEvent& qev = event.quit;
	qev.type = SDL_QUIT;
	SDL_PushEvent(&event);
	return result;
}


FApplication::FApplication(int argc, char** argv) :
	_argc(argc),
	_argv(argv),
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

		platfromContext.Init(_argc, _argv);
		_mainThreadEntry._argc = _argc;
		_mainThreadEntry._argv = _argv;
		_mainThread.init(FMainThreadEntry::ThreadFunc, &_mainThreadEntry);

		if (!platfromContext.Run(_argc, _argv))
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
}

int main(int argc, char** argv)
{
	using namespace Auto3D;
	FApplication app(argc, argv);
	return app.Run();
}

