#include "MassInstantiationSpace.h"
#include "Application.h"
#include "ResourceSystem.h"
#include "Level_0.h"

MassInstantiationSpace::MassInstantiationSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
MassInstantiationSpace::~MassInstantiationSpace()
{
	RemoveScene(0);
}

void MassInstantiationSpace::Awake()
{
	GetSubSystem<ResourceSystem>()->AddResourceDir("../Resource/");
	RegisterScene(new Level_0(_ambient, 0));
}

AUTO_APPLICATION_MAIN(MassInstantiationSpace)