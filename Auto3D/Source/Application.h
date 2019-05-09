#pragma once
#include "Base/Launch.h"
#include "Engine/Engine.h"
namespace Auto3D
{

class Engine;
/// The superclass implementation of the project space, where the engine is implemented
class AUTO_API Application : public Object
{
	REGISTER_OBJECT_CLASS(Application, Object)
public:
	/// Create engine
	Application();
	/// Destructor
	~Application();
	/// Setup before engine initialization.This is a chance to eg. modify the engine parameters.
	///	Call ErrorExit() to terminate without initializing the engine.Called by Application.
	virtual void Init() { }
	/// Setup after engine initialization and before running the main loop.Call ErrorExit() to terminate without running the main loop.
	///	Called by Application.
	virtual void Start() { }
	/// This function is mainly used to _handle non-rendering operations, running once per frame
	virtual void Update() { }
	/// Cleanup after the main loop. Called by Application
	virtual void Stop() { }
	/// This is Engine important funcation init awake runloop and finish run
	int Run();
	/// Show an error message (last log message if empty), terminate the main loop, and set failure exit code.
	void ErrorExit(const String& message = String::EMPTY);
protected:
	/// Auto3D Engine
	AutoPtr<Engine> _engine;
	/// Collected startup error log messages.
	String _startupErrors;
	/// Application exit code.
	int _exitCode;
};

/// @brief : Regisiter application in main function
#define AUTO_APPLICATION_MAIN(_Class) \
int RunApplication() \
{ \
	Thread::SetMainThread();\
    _Class app;\
    return app.Run(); \
} \
AUTO_MAIN(RunApplication())\


}