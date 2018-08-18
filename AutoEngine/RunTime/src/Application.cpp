#include "Application.h"
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
