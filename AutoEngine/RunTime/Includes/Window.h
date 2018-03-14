#ifndef WINDOW_H_
#define WINDOW_H_
#include "OpenGLGather.h"
#include "btConfig.h"
#include "Auto.h"


#if WINDOW_DOT_USE
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif


AUTO_BEGIN
class Window
{
public:
	Window();
	~Window();
	void drawWindow();
	GLFWwindow* createWindow();
	void destoryWindow();
private:
	//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	// member
	GLFWwindow* window;
	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	const char * Title_Name = "Bot V1.0";
	const float DrawColor[4] = { 0.2f,0.3f,0.3f,1.0f };
};
AUTO_END
#endif // WINDOW_H_
