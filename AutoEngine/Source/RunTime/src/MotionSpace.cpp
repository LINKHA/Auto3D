#include "MotionSpace.h"
#include "LevelScene.h"
#include "Scene.h"


namespace Auto3D {
SINGLETON_INSTANCE(SpaceHandle);
MotionSpace::MotionSpace(Ambient* ambient)
	:Super(ambient)
{
	INSTANCE(SpaceHandle).space = this;
}


MotionSpace::~MotionSpace()
{
}

void MotionSpace::RegisterScene(LevelScene* scene)
{
	GetSubSystem<Scene>()->RegisterScene(scene->GetSceneID(), scene);
}

void MotionSpace::RemoveScene(int id)
{
	GetSubSystem<Scene>()->RemoveScene(id);
}

//////////////////////////////////////////////////////////////////////////
//SpaceHandle
//////////////////////////////////////////////////////////////////////////
void SpaceHandle::ModeRunSpace(RunMode runMode)
{
	AssertString(space,"No global space!");

	if (runMode == RunMode::kDefault)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	if (space)
	{
		if (runMode == RunMode::kAwake)
			space->Awake();
		else if (runMode == RunMode::kStart)
			space->Start();
		else if (runMode == RunMode::kUpdate)
			space->Update();
		else if (runMode == RunMode::kFixUpdate)
			space->FixUpdate();
		else if (runMode == RunMode::kFinish)
			space->Finish();
		else if (runMode == RunMode::kDraw)
			space->Draw();
		else
			ErrorString("Space fail to Run.");
	}
}
}


