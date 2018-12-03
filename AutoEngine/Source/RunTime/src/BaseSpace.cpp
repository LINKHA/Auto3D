#include "BaseSpace.h"
#include "MotionSpace.h"
#include "Scene.h"
#include "DebugNew.h"

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
	INSTANCE(SpaceHandle).ModeRunSpace(RunMode::Awake);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kAwake);
}


void BaseSpace::Start()
{
	INSTANCE(SpaceHandle).ModeRunSpace(RunMode::Start);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kStart);
}
void BaseSpace::Update()
{
	INSTANCE(SpaceHandle).ModeRunSpace(RunMode::Update);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kUpdate);
}
void BaseSpace::FixUpdate()
{
	
}
void BaseSpace::Finish()
{
	INSTANCE(SpaceHandle).ModeRunSpace(RunMode::Finish);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kFinish);
}
void BaseSpace::Draw()
{
	INSTANCE(SpaceHandle).ModeRunSpace(RunMode::Draw);

}

}