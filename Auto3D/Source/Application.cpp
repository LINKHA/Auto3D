#include "Application.h"
#include "PlatformSupports/PlatformContext.h"
//#include "Core/ProcessUtils.h"
//#include "Core/ClassRegister.h"

namespace Auto3D
{

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
		if (!PlatfromContext::Get().Run(_argc, _argv))
		{
			ErrorExit();
		}
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

