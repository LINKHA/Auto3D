#pragma once
#include "GLWindow.h"
#include "OpenGLGather.h"
#include "Auto.h"
#include "AtConfig.h"
#include "Ptr.h"
#include "Engine.h"
#include "Launch.h"
#include "Ambient.h"
AUTO_BEGIN
class Application  : public Singleton<Application>
{
public:
	
	Application();
	virtual ~Application();
	virtual int Init();
	virtual int Finish();
	virtual int RunLoop();

	int Run();
protected:
	SharedPtr<Engine> _engine;
private:
	
};
#define AUTO_APPLICATION_MAIN(className) \
int runApplication() \
{ \
    SharedPtr<AUTO::Ambient> context(new AUTO::Ambient()); \
    className work;\
    return work.Launch(); \
} \
AUTO_MAIN(runApplication());


AUTO_END

