#pragma once
#include "OpenGLGather.h"
#include "Singleton.h"
#include "Auto.h"
#include "Ptr.h"
#include "Engine.h"
#include "Launch.h"
#include "Ambient.h"
namespace Auto3D {
class Application  : public Singleton<Application>
{
public:
	
	Application();
	~Application();
	int Init();
	int Awake();
	int RunLoop();
	int Finish();
	void ErrorExit();
	/*
	* @brief : this is Engine important funcation init awake runloop and finish run
	*/
	int Run(Ambient* ambient);
	
private:
	SharedPtr<Engine> _engine;
};
#define AUTO_APPLICATION_MAIN(className) \
int runApplication() \
{ \
    SharedPtr<Auto3D::Ambient> ambient(new Auto3D::Ambient()); \
    className work(ambient);\
    return work.Launch(); \
} \
AUTO_MAIN(runApplication());


}

