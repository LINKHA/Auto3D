#ifndef APPLICATION_H_
#define APPLICATION_H_
#include "Window.h"
#include "OpenGLGather.h"
#include "Auto.h"
#include "InputManager.h"
#include "Shader.h"
#include "stb_image.h"
#include "AtConfig.h"
#include <string.h>

AUTO_BEGIN

class Application 
{
public:
	
	virtual ~Application();
	virtual int init();
	virtual int run();
	virtual int finish();
	virtual int runLoop();
	//virtual bool callBackMessage();
	static Application* getInstance();
	// get Window
private:
	//Member
	Window window;
	GLFWwindow* glfwWindow;
	static Application * m_pApplication;

	//Prevent being called 
	Application();
	Application(const Application&) {}
	Application& operator=(const Application&) {}

};
AUTO_END
#endif // APPLICATION_H_
