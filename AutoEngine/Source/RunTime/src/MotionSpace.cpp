#include "MotionSpace.h"
#include "LevelScene.h"
#include "Scene.h"
#include "Application.h"

#include "NewDef.h"

namespace Auto3D {
MotionSpace::MotionSpace(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	SpaceHandle::Instance().space = SharedPtr<MotionSpace>(this);
}


MotionSpace::~MotionSpace()
{
}

void MotionSpace::RegisterScene(SharedPtr<LevelScene> scene)
{
	GetSubSystem<Scene>()->RegisterScene(scene->GetLevelID(), scene);
}

void MotionSpace::RemoveScene(int id)
{
	GetSubSystem<Scene>()->RemoveScene(id);
}
int MotionSpace::Run() const
{
	Application app;
	return app.Run(_ambient);
}

}


