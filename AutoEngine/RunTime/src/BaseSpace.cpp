#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"
#include "MotionSpace.h"
#include "SpriteTranslucent.h"
AUTO_BEGIN

SINGLETON_INSTANCE(BaseSpace);

BaseSpace::BaseSpace()
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
	INSTANCE(SpriteTranslucentManager).RenderSprite();
}

AUTO_END