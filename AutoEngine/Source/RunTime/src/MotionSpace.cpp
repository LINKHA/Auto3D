#include "MotionSpace.h"
#include "LevelScene.h"
#include "Scene.h"
#include "Application.h"

#include "NewDef.h"

namespace Auto3D {
MotionSpace::MotionSpace(Ambient* ambient)
	:Super(ambient)
{
	SpaceHandle::Instance().space = SharedPtr<MotionSpace>(this);
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
int MotionSpace::Launch() const
{
	return Application::Instance().Run(_ambient);
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


