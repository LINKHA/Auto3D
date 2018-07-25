#include "BaseSpace.h"
#include "GameObjectManager.h"
#include "MotionSpace.h"
#include "SpriteSort.h"
AUTO_BEGIN

BaseSpace::BaseSpace(Ambient* ambient)
	:Super(ambient)
{
}


BaseSpace::~BaseSpace()
{
}

void BaseSpace::Awake()
{
	INSTANCE(SpaceManager).ModeRunSpace(AwakeMode);
	INSTANCE(GameObjectManager).ModeRunGameObject(AwakeMode);
}


void BaseSpace::Start()
{
	INSTANCE(SpaceManager).ModeRunSpace(StartMode);
	INSTANCE(GameObjectManager).ModeRunGameObject(StartMode);
}
void BaseSpace::Update()
{
	INSTANCE(SpaceManager).ModeRunSpace(UpdateMode);
	INSTANCE(GameObjectManager).ModeRunGameObject(UpdateMode);
}
void BaseSpace::FixUpdate()
{
	
}
void BaseSpace::Finish()
{
	INSTANCE(SpaceManager).ModeRunSpace(FinishMode);
	INSTANCE(GameObjectManager).ModeRunGameObject(FinishMode);
}
void BaseSpace::Draw()
{
	INSTANCE(SpaceManager).ModeRunSpace(DrawMode);
	INSTANCE(GameObjectManager).ModeRunGameObject(DrawMode);
	INSTANCE(SpriteSort).RenderSprite();
}

AUTO_END