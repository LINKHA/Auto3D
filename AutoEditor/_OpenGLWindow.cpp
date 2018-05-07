#include "_OpenGLWindow.h"
#include "OpenGLGather.h"/*
_OpenGLWindow::_OpenGLWindow()
{
}

_OpenGLWindow::~_OpenGLWindow()
{
}

void _OpenGLWindow::_initializeGL()
{

	//设置全局变量  
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void _OpenGLWindow::_paintGL()
{
	//清理屏幕  
	glClear(GL_COLOR_BUFFER_BIT);
}

void _OpenGLWindow::_resizeGL(int width, int height)
{
}*/
void _initializeGL()
{
	if (!gladLoadGL())
	{
		
	}
	//设置全局变量  
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void _paintGL()
{
	//清理屏幕  
	glClear(GL_COLOR_BUFFER_BIT);
}

void _resizeGL(int width, int height)
{
}