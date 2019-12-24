#pragma once
#include "Core/Object/Object.h"
#include "Core/ConslonDef.h"

#if defined(_WIN32)
#	include <windows.h>
#endif
#ifdef _MSC_VER
#	include <crtdbg.h>
#endif

namespace Auto3D
{

class AEngine;
/// The superclass implementation of the project space, where the engine is implemented
class AUTO_API AApplication : public AObject
{
	DECLARE_CLASS(AApplication, AObject)

	DECLARE_RTTR_CLASS(AApplication, AObject)
public:
	/// Create engine
	AApplication();
	/// Destructor
	~AApplication();
	/// Setup before engine initialization.This is a chance to eg. modify the engine parameters.
	///	Call ErrorExit() to terminate without initializing the engine.Called by AApplication.
	virtual void Init() { }
	/// Setup after engine initialization and before running the main loop.Call ErrorExit() to terminate without running the main loop.
	///	Called by AApplication.
	virtual void Start() { }
	/// This function is mainly used to _handle non-rendering operations, running once per frame
	virtual void Update() { }
	/// Cleanup after the main loop. Called by AApplication
	virtual void Stop() { }
	/// This function is the AUIModule rendered after the render function at the top
	virtual void UIDraw() { }
	/// This is AEngine important funcation init awake runloop and finish run
	int Run();
	/// Show an error message (last log message if empty), terminate the main loop, and set failure exit code.
	void ErrorExit(const FString& message = FString::EMPTY);
protected:
	/// Auto3D AEngine
	TUniquePtr<AEngine> _engine;
	/// Collected startup error log messages.
	FString _startupErrors;
	/// AApplication exit code.
	int _exitCode;
};

#if defined(_MSC_VER) && defined(_DEBUG) && defined(AUTO_WIN32_CONSOLE)
#	define AUTO_MAIN(function) \
		int main(int argc, char** argv) \
		{ \
			DETECT_MEMORY_LEAKS();\
			int flag = function;\
			DUMP_MEMORY_LEAKS();\
			return flag;\
		}
#elif defined(_MSC_VER) && defined(_DEBUG) && !defined(AUTO_WIN32_CONSOLE)
#	define AUTO_MAIN(_function) \
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
		{ \
			DETECT_MEMORY_LEAKS();\
			int flag = _function;\
			DUMP_MEMORY_LEAKS();\
			return flag;\
		}
#elif defined(_MSC_VER) && NDEBUG &&!defined(AUTO_WIN32_CONSOLE)
#	define AUTO_MAIN(_function) \
		HINSTANCE g_hInstance;HINSTANCE g_prevInstance;PSTR g_cmdLine;int g_showCmd;\
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
		{ \
			int flag = _function;\
			return flag;\
		}
#elif defined(__ANDROID__) || defined(IOS)
#	define AUTO_MAIN(_function) \
		extern "C" __attribute__((visibility("default"))) int SDL_main(int argc, char** argv); \
		int SDL_main(int argc, char** argv) \
		{ \
			int flag = _function;\
			return flag;\
		}
#else
#	define AUTO_MAIN(_function) \
		int main(int argc, char** argv) \
		{ \
			int flag = _function;\
			return flag;\
		}
#endif

/// @brief : Regisiter application in main function
#define AUTO_APPLICATION_MAIN(_Class) \
int RunApplication() \
{ \
	FThread::SetMainThread();\
    _Class app;\
    return app.Run(); \
} \
AUTO_MAIN(RunApplication())\


}