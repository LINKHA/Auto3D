#include "WorkSpace.h"



WorkSpace::WorkSpace()
{
}


WorkSpace::~WorkSpace()
{
}

int WorkSpace::Launch()
{
	Applacation app;
	return app.run();
}
