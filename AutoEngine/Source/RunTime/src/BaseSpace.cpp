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
	SpaceHandle::Instance().ModeRunSpace(RunMode::Awake);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kAwake);
}


void BaseSpace::Start()
{
	SpaceHandle::Instance().ModeRunSpace(RunMode::Start);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kStart);
}
void BaseSpace::Update()
{
	SpaceHandle::Instance().ModeRunSpace(RunMode::Update);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kUpdate);
}
void BaseSpace::FixUpdate()
{
	
}
void BaseSpace::Finish()
{
	SpaceHandle::Instance().ModeRunSpace(RunMode::Finish);
	//GetSubSystem<Scene>()->ModeRunLevel(RunMode::kFinish);
}
void BaseSpace::Draw()
{
	SpaceHandle::Instance().ModeRunSpace(RunMode::Draw);

}

}