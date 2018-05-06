#include "AutoEditor.h"
#include <QtWidgets/QApplication>
#include "Application.h"

USING_AUTO

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	AutoEditor w;
	w.show();
	return Application::Instance().run();
	//return a.exec();
}
