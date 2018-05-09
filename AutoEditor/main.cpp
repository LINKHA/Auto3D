#include "AutoEditor.h"
#include <QtWidgets/QApplication>
#include "OpenGLWindow.h"
#define AUTO_EDITOR

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AutoEditor w;
	w.show();
	return a.exec();
}
