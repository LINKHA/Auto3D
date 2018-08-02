#include "Application.h"
#include "stb_image.h"
namespace Auto3D {

SINGLETON_INSTANCE(Application);
Application::Application()
{
}
Application::~Application()
{
}

int Application::Run(Ambient* ambient)
{
	try 
	{
		_engine = new Engine(ambient);
		if (Awake() == APP_ERROR)
		{
			ErrorExit();
			return APP_ERROR;
		}
		if (Init() == APP_ERROR)
		{
			ErrorExit();
			return APP_ERROR;
		}
		if (RunLoop() == APP_ERROR)
		{
			ErrorExit();
			return APP_ERROR;
		}
		return Finish();
	}
	catch (std::bad_alloc&)
	{
		ErrorString("An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
}

int Application::Awake()
{
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine\n");
		return APP_ERROR;
	}
	return APP_NORMAL;
}
int Application::Init()
{
	_engine->Init();
	return APP_NORMAL;
}

int Application::RunLoop()
{
	while (!_engine->IsExiting())
		_engine->RunFrame();
	return APP_NORMAL;
}
int Application::Finish()
{
	_engine->Exit();
	return APP_NORMAL;
}
 void Application::ErrorExit()
{
	 _engine->Exit();
}
}
