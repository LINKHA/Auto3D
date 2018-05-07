#include "AutoEditor.h"
#include <QtWidgets/QApplication>
#include "OpenGLWindow.h"
//#include "Application.h"

#define AUTO_EDITOR
//USING_AUTO

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLWindow w;
	w.show();
	//return Application::Instance().run();
	return a.exec();
}
