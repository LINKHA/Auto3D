#include "Application.h"
#include "stb_image.h"
AUTO_BEGIN

SINGLETON_INSTANCE(Application);
Application::Application()
{
	INSTANCE(GLWindow).CreateGameWindow();
	_ambient = new AUTO::Ambient();
	_engine = new Engine(_ambient);
}
Application::~Application()
{
}
/*
* @Application::run
* this is Engine important funcation,
* Determine the number of frames based on the speed of this function
* Run once per frame
*/
int Application::Run()
{
	try 
	{
		if (Init() == APP_ERROR)
		{
			ErrorExit();
			return APP_ERROR;
		}
		if (Awake() == APP_ERROR)
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

int Application::Init()
{
	stbi_set_flip_vertically_on_load(true);

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine\n");
		return APP_ERROR;
	}
	return APP_NORMAL;
}
int Application::Awake()
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
AUTO_END
