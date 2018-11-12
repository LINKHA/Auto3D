#include "SSAOSpace.h"
#include "Application.h"
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
	RegisterScene(new Level_0(_ambient, 0));
}


int SSAOSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(SSAOSpace)