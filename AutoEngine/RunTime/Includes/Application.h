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
#include "Application.h"
#include "Mesh.h"
#include <string.h>
AUTO_BEGIN
class Application  : public Singleton<Application>
{
public:
	
	virtual ~Application();
	virtual int init();
	virtual int run();
	virtual int finish();
	virtual int runLoop();
	//virtual bool callBackMessage();
	Application();

	Camera m_camera;
private:
	GLWindow window;
	GLFWwindow* glfwWindow;
	
};
AUTO_END
#endif // APPLICATION_H_
