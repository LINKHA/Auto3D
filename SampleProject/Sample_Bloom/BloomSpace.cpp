#include "BloomSpace.h"
#include "Application.h"
#include "Level_0.h"

BloomSpace::BloomSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
BloomSpace::~BloomSpace()
{
	RemoveScene(0);
}

void BloomSpace::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}
AUTO_APPLICATION_MAIN(BloomSpace)
