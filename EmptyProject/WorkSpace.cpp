#include "WorkSpace.h"
#include "Application.h"
#include "Level_0.h"

WorkSpace::WorkSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
WorkSpace::~WorkSpace()
{
	RemoveLevel(0);
}

void WorkSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}


int WorkSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(WorkSpace)