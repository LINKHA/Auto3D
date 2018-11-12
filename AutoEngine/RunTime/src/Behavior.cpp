#include "Behavior.h"
#include "Scene.h"

namespace Auto3D {


Behavior::Behavior(Ambient* ambient)
	:Super(ambient)
{
}


Behavior::~Behavior()
{
}

void Behavior::Awake()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kAwake);
}

void Behavior::Start()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kStart);
}

void Behavior::Update()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kUpdate);
}

void Behavior::FixUpdate()
{

}

void Behavior::Finish()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::kFinish);
}

void Behavior::Draw()
{

}

}