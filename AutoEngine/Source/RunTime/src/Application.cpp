#include "Application.h"
#include "NewDef.h"

namespace Auto3D {

Application::Application(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}
Application::~Application()
{
}

int Application::Run()
{
	try 
	{
		_engine = MakeShared<Engine>(_ambient);
		setStates(AppStates::Initing);
		Init();
		_engine->Init();

		setStates(AppStates::Running);
		Start();
		while (!_engine->IsExiting())
			_engine->RunFrame();

		setStates(AppStates::Exit);
		Stop();
		_engine->Exit();

	}
	catch (std::bad_alloc&)
	{
		ErrorString("An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}

}

}
