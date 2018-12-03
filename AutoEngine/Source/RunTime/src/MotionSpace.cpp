#include "MotionSpace.h"
#include "LevelScene.h"
#include "Scene.h"
#include "DebugNew.h"

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

	if (runMode == RunMode::Default)
	{
		ErrorString("Space fail to Run.");
		return;
	}
	if (space)
	{
		if (runMode == RunMode::Awake)
			space->Awake();
		else if (runMode == RunMode::Start)
			space->Start();
		else if (runMode == RunMode::Update)
			space->Update();
		else if (runMode == RunMode::FixUpdate)
			space->FixUpdate();
		else if (runMode == RunMode::Finish)
			space->Finish();
		else if (runMode == RunMode::Draw)
			space->Draw();
		else
			ErrorString("Space fail to Run.");
	}
}
}


