#include "_OpenGLWindow.h"
#include "GLGather.h"
#include "LogAssert.h"
using namespace Auto3D;
void _initializeGL()
{
	
	ErrorIfString(!gladLoadGL(), "Failed to initialize GLAD from Editor!\n");
	
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void _paintGL()
{
	//If MouseEvent or KeyEvent input paint,this is low to frame

	glClear(GL_COLOR_BUFFER_BIT);
}

void _resizeGL(int width, int height)
{
}