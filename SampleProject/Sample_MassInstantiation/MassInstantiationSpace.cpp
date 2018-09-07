#include "MassInstantiationSpace.h"
#include "Application.h"
#include "Level_0.h"

MassInstantiationSpace::MassInstantiationSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
MassInstantiationSpace::~MassInstantiationSpace()
{
	RemoveLevel(0);
}

void MassInstantiationSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int MassInstantiationSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(MassInstantiationSpace)