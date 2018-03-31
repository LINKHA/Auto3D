#include "WorkSpace.h"



WorkSpace::WorkSpace()
{
}


WorkSpace::~WorkSpace()
{
}

int WorkSpace::Launch()
{
	return Application::getInstance()->run();
}
