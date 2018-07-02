#include "GLWindow.h"
#include "Monitors.h"
AUTO_BEGIN

SINGLETON_INSTANCE(GLWindow);

void size_callback(GLFWwindow* window, int width, int height)
{
	INSTANCE(GLWindow).UpdateWindowRectInt(width, height);
}

//public funcation
GLWindow::GLWindow()
	: window(nullptr)
{
	GrInit(3, 3);
	DrawColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
	TitleName = "Auto V0.0";
	WindowRect.x = 0;
	WindowRect.y = 0;
	WindowRect.width = 1200;
	WindowRect.height = 800;
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
/**
* @brief delete all resource
*/
void GLWindow::DestoryWindow()
{
	GrDeleteResource();
}

void GLWindow::CreateGameWindow()
{
	int t_Width, t_Height;
	t_Width = INSTANCE(Monitors).GetMonitorsWidthIndex(0);
	t_Height = INSTANCE(Monitors).GetMonitorsHeightWithIndex(0);
	if (isFullScreen)
	{
		WindowRect.width = t_Width;
		WindowRect.height = t_Height;
	}
	else
	{
		if (isCenter)
		{
			WindowRect.x = t_Width / 2;
			WindowRect.y = t_Height / 2;
		}
	}
	GLFWmonitor* pMonitor = isFullScreen ? glfwGetPrimaryMonitor() : NULL;
	GrCreateWindow(&window, WindowRect.width, WindowRect.height, TitleName, pMonitor);

	glfwSetWindowPos(window, WindowRect.x - WindowRect.width/2, WindowRect.y - WindowRect.height / 2);
	GrFrameSizeCallBack(window, size_callback);
}



AUTO_END