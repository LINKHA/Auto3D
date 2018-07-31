#pragma once
#include "OpenGLGather.h"
#include "Singleton.h"
#include "Auto.h"
#include "AtConfig.h"
#include "Ptr.h"
#include "Engine.h"
#include "Launch.h"
#include "Ambient.h"
namespace Auto3D {
class Application  : public Singleton<Application>
{
public:
	
	Application();
	virtual ~Application();
	virtual int Init();
	virtual int Awake();
	virtual int RunLoop();
	virtual int Finish();
	virtual void ErrorExit();
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

