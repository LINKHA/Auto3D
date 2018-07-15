#pragma once
#include "GLWindow.h"
#include "OpenGLGather.h"
#include "Auto.h"
#include "InputManager.h"
#include "Shader.h"
#include "AtConfig.h"
#include "Math/AUMath.h"
#include "Texture2D.h"
#include "Monitors.h"

AUTO_BEGIN
class Application  : public Singleton<Application>
{
public:
	
	virtual ~Application();
	virtual int Init();
	virtual int Run();
	virtual int Finish();
	virtual int RunLoop();
	//virtual bool callBackMessage();
	Application();
private:
	
};
AUTO_END

