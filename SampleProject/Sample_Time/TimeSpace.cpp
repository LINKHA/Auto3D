#include "TimeSpace.h"
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
AUTO_APPLICATION_MAIN(TimeSpace)