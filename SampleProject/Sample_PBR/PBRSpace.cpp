#include "PBRSpace.h"
#include "Application.h"
#include "Level_0.h"

PBRSpace::PBRSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
PBRSpace::~PBRSpace()
{
	RemoveLevel(0);
}

void PBRSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int PBRSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(PBRSpace)