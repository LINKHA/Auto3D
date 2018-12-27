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
	GetSubSystem<Scene>()->RegisterScene(scene->GetLevelID(), scene);
}

void MotionSpace::RemoveScene(int id)
{
	GetSubSystem<Scene>()->RemoveScene(id);
}
int MotionSpace::Launch() const
{
	return Application::Instance().Run(_ambient);
}

}


