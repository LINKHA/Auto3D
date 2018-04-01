#include "Window.h"



AUTO_BEGIN

void size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	GLViewPort(0, 0, width, height);
}


//public funcation
Window::Window()
	:window(nullptr)
{
	DrawColor = Color(m_DrawColorData[0], m_DrawColorData[1], m_DrawColorData[2], m_DrawColorData[3]);
	GLInit(3, 3);
	
}


Window::~Window()
{
	
}

void Window::drawWindow()
{
	GLClearColor(DrawColor);
	/// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	/// -------------------------------------------------------------------------------
	GLRunApplication(window);

}
GLFWwindow* Window::createWindow()
{
	GLCreateWindow(&window, SCR_WIDTH, SCR_HEIGHT, Title_Name);

	GLFrameSizeCallBack(window, size_callback);

	return window;
}
void Window::destoryWindow()
{
	GLDeleteResource();
}



AUTO_END