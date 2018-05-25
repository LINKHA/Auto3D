#ifndef APPLICATION_H_
#define APPLICATION_H_
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

	GLWindow window;
private:

	GLFWwindow* glfwWindow;
	
};
AUTO_END
#endif // APPLICATION_H_
