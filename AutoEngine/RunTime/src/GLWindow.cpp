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
	:window(nullptr)
{
	DrawColor = Color(m_DrawColorData[0], m_DrawColorData[1], m_DrawColorData[2], m_DrawColorData[3]);
	GrInit(3, 3);
	
}


GLWindow::~GLWindow()
{
	
}

void GLWindow::drawWindow()
{
	GrClearColor(DrawColor);
	
}
void GLWindow::runLoopOver()
{	
	GrSwapBuffers(window);
	/*get Events*/
	GrPollEvents();
}
GLFWwindow* GLWindow::createWindow()
{
	GrCreateWindow(&window, SCR_WIDTH, SCR_HEIGHT, Title_Name);

	GrFrameSizeCallBack(window, size_callback);

	return window;
}
/**
* @brief delete all resource
*/
void GLWindow::destoryWindow()
{
	GrDeleteResource();
}



AUTO_END