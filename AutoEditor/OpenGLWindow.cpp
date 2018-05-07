#include "OpenGLWindow.h"
#include "_OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(QWidget *parent)
	: QOpenGLWidget(parent)
{
	format = QSurfaceFormat();
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);
	setFormat(format);
}

OpenGLWindow::~OpenGLWindow()
{
}
void OpenGLWindow::initializeGL()
{
	_initializeGL();
}
void OpenGLWindow::resizeGL(int width, int height)
{
	_resizeGL(width,height);
}
void OpenGLWindow::paintGL()
{
	_paintGL();
}