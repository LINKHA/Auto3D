#pragma once
#include "GLWindow.h"
#include "OpenGLGather.h"
#include "Auto.h"
#include "AtConfig.h"
#include "Ptr.h"
#include "Engine.h"
AUTO_BEGIN
class Application  : public Singleton<Application>
{
public:
	

	virtual ~Application();
	virtual int Init();
	virtual int Finish();
	virtual int RunLoop();

	int Run();
	Application();
protected:
	SharedPtr<Engine> _engine;
private:
	
};
AUTO_END

