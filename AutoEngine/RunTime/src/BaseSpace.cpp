#include "BaseSpace.h"
#include "MotionSpace.h"
#include "Scene.h"
namespace Auto3D {

BaseSpace::BaseSpace(Ambient* ambient)
	:Super(ambient)
{
}


BaseSpace::~BaseSpace()
{
}

void BaseSpace::Awake()
{
	INSTANCE(SpaceManager).ModeRunSpace(RunMode::kAwake);
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kAwake);
}


void BaseSpace::Start()
{
	INSTANCE(SpaceManager).ModeRunSpace(RunMode::kStart);
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kStart);
}
void BaseSpace::Update()
{
	INSTANCE(SpaceManager).ModeRunSpace(RunMode::kUpdate);
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kUpdate);
}
void BaseSpace::FixUpdate()
{
	
}
void BaseSpace::Finish()
{
	INSTANCE(SpaceManager).ModeRunSpace(RunMode::kFinish);
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kFinish);
}
void BaseSpace::Draw()
{
	INSTANCE(SpaceManager).ModeRunSpace(RunMode::kDraw);

}

}