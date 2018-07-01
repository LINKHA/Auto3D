#include "MotionSpace.h"


AUTO_BEGIN
SINGLETON_INSTANCE(SpaceManager);
MotionSpace::MotionSpace()
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
void SpaceManager::ModeRunSpace(RunMode runMode)
{
	if (runMode == DefaultMode)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	for (auto i = spaces.begin(); i != spaces.end(); i++)
	{
		MotionSpace* space = *i;
		if (space)
		{
			if (runMode == AwakeMode)
				space->Awake();
			else if (runMode == StartMode)
				space->Start();
			else if (runMode == UpdateMode)
				space->Update();
			else if (runMode == FixUpdateMode)
				space->FixUpdate();
			else if (runMode == FinishMode)
				space->Finish();
			else if (runMode == DrawMode)
				space->Draw();
			else
				ErrorString("Space fail to Run.");
		}
	}
}
AUTO_END


