#include "GLWindow.h"
#include "Monitors.h"
#include "Icon.h"
#include "AtConfig.h"

AUTO_BEGIN

SINGLETON_INSTANCE(GLWindow);

void size_callback(GLFWwindow* window, int width, int height)
{
	INSTANCE(GLWindow).UpdateWindowRectInt(width, height);
}

//public funcation
GLWindow::GLWindow()
	: _window(nullptr)
{
	GrInit(3, 3);
	_drawColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
	_titleName = "Auto V0.0";
	_windowRect.x = 0;
	_windowRect.y = 0;
	_windowRect.width = 1280;
	_windowRect.height = 720;
}

GLWindow::~GLWindow()
{	
}

void GLWindow::DrawWindow()
{
	GrClearColor(_drawColor);
	
}
void GLWindow::RunLoopOver()
{	
	GrSwapBuffers(_window);
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
	int width, height;
	width = INSTANCE(Monitors).GetMonitorsWidthIndex(0);
	height = INSTANCE(Monitors).GetMonitorsHeightWithIndex(0);
	if (_isFullScreen)
	{
		_windowRect.width = width;
		_windowRect.height = height;
	}
	else
	{
		if (_isCenter)
		{
			_windowRect.x = width / 2;
			_windowRect.y = height / 2;
		}
	}
	GLFWmonitor* pMonitor = _isFullScreen ? glfwGetPrimaryMonitor() : NULL;
	GrCreateWindow(&_window, _windowRect.width, _windowRect.height, _titleName, pMonitor);
	glfwSetWindowPos(_window, _windowRect.x - _windowRect.width/2, _windowRect.y - _windowRect.height / 2);
	GrFrameSizeCallBack(_window, size_callback);

	//icon_load(window, AtConfig::source_path + "logo.png");
}



AUTO_END