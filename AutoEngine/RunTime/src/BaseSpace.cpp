#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "BaseLight.h"
#include "MotionSpace.h"
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
void BaseSpace::Draw(Camera* camera)
{
	INSTANCE(SpaceManager).ModeRunSpace(DrawMode);
	INSTANCE(GameObjectManager).ModeRunGameObject(DrawMode, camera);
}

AUTO_END