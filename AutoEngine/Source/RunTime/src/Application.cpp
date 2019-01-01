#include "Application.h"
#include "NewDef.h"

namespace Auto3D {

Application::Application()
{
}
Application::~Application()
{
}

bool Application::Run(SharedPtr<Ambient> ambient)
{
	try 
	{
		_engine = MakeShared<Engine>(ambient);
		if (!Init())
		{
			ErrorExit();
			return false;
		}
		if (!RunLoop())
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
	return true;
}

bool Application::RunLoop()
{
	setStates(AppStates::Running);
	while (!_engine->IsExiting())
		_engine->RunFrame();
	return true;
}
bool Application::Finish()
{
	setStates(AppStates::Exit);
	_engine->Exit();

	return false;
}
 void Application::ErrorExit()
{
	 setStates(AppStates::ErrorExit);
	 _engine->Exit();
}

}
