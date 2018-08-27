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
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kAwake);
	GetSubSystem<Scene>()->ModeRunNode(NodeRunMode::kAwake);
}


void BaseSpace::Start()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kStart);
	GetSubSystem<Scene>()->ModeRunNode(NodeRunMode::kStart);
}
void BaseSpace::Update()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kUpdate);
	GetSubSystem<Scene>()->ModeRunNode(NodeRunMode::kUpdate);
}
void BaseSpace::FixUpdate()
{
	
}
void BaseSpace::Finish()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kFinish);
	GetSubSystem<Scene>()->ModeRunNode(NodeRunMode::kFinish);
}
void BaseSpace::Draw()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kDraw);

}

}