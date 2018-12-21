#include "UISapce.h"
#include "ResourceSystem.h"
#include "level_0.h"
UISapce::UISapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
UISapce::~UISapce()
{
	RemoveScene(0);
}
void UISapce::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}


AUTO_APPLICATION_MAIN(UISapce)