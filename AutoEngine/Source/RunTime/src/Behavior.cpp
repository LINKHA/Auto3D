#include "Behavior.h"
#include "Scene.h"
#include "DebugNew.h"

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
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::Awake);
}

void Behavior::Start()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::Start);
}

void Behavior::Update()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::Update);
}

void Behavior::FixUpdate()
{

}

void Behavior::Finish()
{
	GetSubSystem<Scene>()->ModeRunLevel(RunMode::Finish);
}

void Behavior::Draw()
{

}

}