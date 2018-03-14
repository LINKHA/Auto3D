#ifndef APPLACATION_H_
#define APPLACATION_H_
#include "Window.h"
#include "OpenGLGather.h"
#include "Auto.h"
AUTO_BEGIN
class Applacation 
{
public:
	Applacation();
	virtual ~Applacation();
	virtual int init();
	virtual int run();
	virtual int finish();
	virtual int runLoop();
	//virtual bool callBackMessage();

// get Window
private:
	//Member
	Window* window;
	GLFWwindow* glfwWindow;
};
AUTO_END
#endif // APPLACATION_H_
