#include "MotionSpace.h"


namespace Auto3D {
SINGLETON_INSTANCE(SpaceManager);
MotionSpace::MotionSpace(Ambient* ambient)
	:Super(ambient)
{
	INSTANCE(SpaceManager).RegisterSpace(this);
}
MotionSpace::~MotionSpace()
{
}
//////////////////////////////////////////////////////////////////////////
//Class::SpaceRunMode
//////////////////////////////////////////////////////////////////////////
void SpaceManager::RegisterSpace(MotionSpace * space)
{
	spaces.push_back(space);
}
void SpaceManager::ModeRunSpace(MotionRunMode runMode)
{
	if (runMode == MotionRunMode::kDefault)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	for (auto i = spaces.begin(); i != spaces.end(); i++)
	{
		MotionSpace* space = *i;
		if (space)
		{
			if (runMode == MotionRunMode::kAwake)
				space->Awake();
			else if (runMode == MotionRunMode::kStart)
				space->Start();
			else if (runMode == MotionRunMode::kUpdate)
				space->Update();
			else if (runMode == MotionRunMode::kFixUpdate)
				space->FixUpdate();
			else if (runMode == MotionRunMode::kFinish)
				space->Finish();
			else if (runMode == MotionRunMode::kDraw)
				space->Draw();
			else
				ErrorString("Space fail to Run.");
		}
	}
}
}


