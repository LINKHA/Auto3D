#include "Application.h"
#include "DebugNew.h"

namespace Auto3D {

SINGLETON_INSTANCE(Application);
Application::Application()
{
}
Application::~Application()
{
	SAFE_DELETE(_engine);
}

bool Application::Run(Ambient* ambient)
{
	try 
	{
		_engine = new Engine(ambient);
		if (Init())
		{
			ErrorExit();
			return false;
		}
		if (RunLoop() == false)
		{
			ErrorExit();
			return false;
		}
		return Finish();
	}
	catch (std::bad_alloc&)
	{
		ErrorString("An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
}
bool Application::Init()
{
	setStates(AppStates::Initing);
	_engine->Init();
	return false;
}

bool Application::RunLoop()
{
	setStates(AppStates::Running);
	while (!_engine->IsExiting())
		_engine->RunFrame();
	return false;
}
bool Application::Finish()
{
	setStates(AppStates::Exit);
	_engine->Exit();
	_CrtDumpMemoryLeaks();
	return false;
}
 void Application::ErrorExit()
{
	 setStates(AppStates::ErrorExit);
	 _engine->Exit();
}

}
