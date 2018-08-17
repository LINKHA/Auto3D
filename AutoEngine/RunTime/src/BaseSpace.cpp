#include "BaseSpace.h"
#include "GameObjectManager.h"
#include "MotionSpace.h"
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
	INSTANCE(GameObjectManager).ModeRunGameObject(GameObjectRunMode::kAwake);
}


void BaseSpace::Start()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kStart);
	INSTANCE(GameObjectManager).ModeRunGameObject(GameObjectRunMode::kStart);
}
void BaseSpace::Update()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kUpdate);
	INSTANCE(GameObjectManager).ModeRunGameObject(GameObjectRunMode::kUpdate);
}
void BaseSpace::FixUpdate()
{
	
}
void BaseSpace::Finish()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kFinish);
	INSTANCE(GameObjectManager).ModeRunGameObject(GameObjectRunMode::kFinish);
}
void BaseSpace::Draw()
{
	INSTANCE(SpaceManager).ModeRunSpace(MotionRunMode::kDraw);
	//INSTANCE(GameObjectManager).ModeRunGameObject(DrawMode);

}

}