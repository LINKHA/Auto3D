#include "SSAOSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "Level_0.h"


SSAOSpace::SSAOSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
SSAOSpace::~SSAOSpace()
{
	RemoveScene(0);
}

void SSAOSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(SSAOSpace)