#include "MoreCameraSpace.h"
#include "Application.h"
#include "Level_0.h"

MoreCameraSpace::MoreCameraSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}
MoreCameraSpace::~MoreCameraSpace()
{
	RemoveLevel(0);
}

void MoreCameraSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}
int MoreCameraSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(MoreCameraSpace)