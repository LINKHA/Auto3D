#include "TimeSpace.h"
#include "Application.h"
#include "Level_0.h"


TimeSpace::TimeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
TimeSpace::~TimeSpace()
{
	RemoveScene(0);
}
void TimeSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}
int TimeSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(TimeSpace)