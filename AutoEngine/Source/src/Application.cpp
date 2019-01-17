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
		Init();
		_engine->Init();

		Start();
		while (!_engine->IsExiting())
		{
			_engine->Update();
			Update();
			_engine->Render();
			_engine->FrameFinish();
			
		}
			
		Stop();
		_engine->Exit();

	}
	catch (std::bad_alloc&)
	{
		ErrorString("An application that has an out-of-memory condition will exit immediately.");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

}
