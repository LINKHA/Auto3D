#pragma once
#include "Singleton.h"
#include "Auto.h"
#include "Engine.h"
#include "Launch.h"
#include "Ambient.h"
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

class Application  : public Singleton<Application>
{
public:
	Application();
	~Application();
	/**
	* @brief : Init application
	*/
	bool Init();
	/**
	* @brief : Run frame and update logic
	*/
	bool RunLoop();
	/**
	* @brief : Finish exit application
	*/
	bool Finish();
	/**
	* @brief : Error exit application
	*/
	void ErrorExit();
	/*
	* @brief : this is Engine important funcation init awake runloop and finish run
	*/
	bool Run(Ambient* ambient);
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
	sharedPtr<Engine> _engine;
	AppStates _appStates;
};

/// @brief : Regisiter application in main function
#define AUTO_APPLICATION_MAIN(className) \
int runApplication() \
{ \
    sharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); \
    className work(ambient);\
    return work.Launch(); \
} \
AUTO_MAIN(runApplication());


}

