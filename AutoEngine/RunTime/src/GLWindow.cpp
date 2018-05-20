#include "GLWindow.h"

AUTO_BEGIN

void size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	GrViewPort(0, 0, width, height);
}

//public funcation
GLWindow::GLWindow()
	: window(nullptr)
	
{
	GrInit(3, 3);
	DrawColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
}

GLWindow::~GLWindow()
{	
}

void GLWindow::DrawWindow()
{
	GrClearColor(DrawColor);
	
}
void GLWindow::RunLoopOver()
{	
	GrSwapBuffers(window);
	/*get Events*/
	GrPollEvents();
}
GLFWwindow* GLWindow::CreateGameWindow()
{
	GrCreateWindow(&window, SCR_WIDTH, SCR_HEIGHT, Title_Name);

	GrFrameSizeCallBack(window, size_callback);

	return window;
}
/**
* @brief delete all resource
*/
void GLWindow::DestoryWindow()
{
	GrDeleteResource();
}



AUTO_END