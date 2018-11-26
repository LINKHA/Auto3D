#pragma once
#include "Singleton.h"
#include "Auto.h"
#include "Engine.h"
#include "Launch.h"
#include "Ambient.h"
namespace Auto3D {
class Application  : public Singleton<Application>
{
public:
	Application();
	~Application();
	/**
	* @brief : Init application
	*/
	int Init();
	/**
	* @brief : Run frame and update logic
	*/
	int RunLoop();
	/**
	* @brief : Finish exit application
	*/
	int Finish();
	/**
	* @brief : Error exit application
	*/
	void ErrorExit();
	/*
	* @brief : this is Engine important funcation init awake runloop and finish run
	*/
	int Run(Ambient* ambient);
	
private:
	sharedPtr<Engine> _engine;
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

