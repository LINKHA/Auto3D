#include "BloomSpace.h"
#include "Application.h"
#include "Level_0.h"

BloomSpace::BloomSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
BloomSpace::~BloomSpace()
{
	RemoveLevel(0);
}

void BloomSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

int BloomSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(BloomSpace)
