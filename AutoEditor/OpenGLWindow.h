#pragma once

#include <QObject>
#include <QOpenGLWidget>

class OpenGLWindow : public QOpenGLWidget
{
	Q_OBJECT

public:
	OpenGLWindow(QWidget *parent = Q_NULLPTR);
	~OpenGLWindow();
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
private:
	QSurfaceFormat format;
};
