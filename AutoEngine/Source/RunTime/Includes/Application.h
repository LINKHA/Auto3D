#pragma once
#include "Singleton.h"
#include "Auto.h"
#include "Engine.h"
#include "Ambient.h"
#include "Scene.h"
#include "Launch.h"

namespace Auto3D {

enum class AppStates
{
	Initing,
	Running,
	Pauseing,
	Stopping,
	Exit,
	ErrorExit,
};

class Application : public Object
{
	REGISTER_OBJECT_CLASS(Application, Object)
public:
	Application(SharedPtr<Ambient> ambient);
	/**
	* @brief : Setup before engine initialization. 
	*		This is a chance to eg. modify the engine parameters. 
	*		Call ErrorExit() to terminate without initializing the engine.
	*		Called by Application.
	*/
	virtual void Init() { }
	/**
	* @brief :Setup after engine initialization and before running the main loop.Call ErrorExit() to terminate without running the main loop.
	*		Called by Application.
	*/
	virtual void Start() { }
	/**
	* @brief : Cleanup after the main loop. Called by Application
	*/
	virtual void Stop() { }
	/*
	* @brief : this is Engine important funcation init awake runloop and finish run
	*/
	int Run();
	/**
	* @brief : Return application states for AppStates type
	*/
	const AppStates GetStates() const { return _appStates; }

private:
	/**
	* @brief : Set states fot application
	*/
	void setStates(AppStates states) { _appStates = states; }
private:
	SharedPtr<Engine> _engine;
	AppStates _appStates;
};

/// @brief : Regisiter application in main function
#define AUTO_APPLICATION_MAIN(_Class) \
int runApplication() \
{ \
    SharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); \
    _Class app(ambient);\
    return app.Run(); \
} \
AUTO_MAIN(runApplication())\



}

