#include "TimeSpace.h"
#include "Level_0.h"
#include "ResourceSystem.h"

TimeSpace::TimeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
TimeSpace::~TimeSpace()
{
	RemoveScene(0);
}
void TimeSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(TimeSpace)