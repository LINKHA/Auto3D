#include "Behavior.h"
#include "tScene.h"
#include "NewDef.h"

namespace Auto3D {


Behavior::Behavior(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


Behavior::~Behavior()
{
}

void Behavior::Awake()
{
	GetSubSystem<tScene>()->ModeRunLevel(RunMode::Awake);
}

void Behavior::Start()
{
	GetSubSystem<tScene>()->ModeRunLevel(RunMode::Start);
}

void Behavior::Update()
{
	GetSubSystem<tScene>()->ModeRunLevel(RunMode::Update);
}

void Behavior::FixUpdate()
{

}

void Behavior::Finish()
{
	GetSubSystem<tScene>()->ModeRunLevel(RunMode::Finish);
}

void Behavior::Draw()
{

}

}