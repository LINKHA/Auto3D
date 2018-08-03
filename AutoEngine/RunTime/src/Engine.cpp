#include "Engine.h"
#include "GameObject.h"
#include "Shadow.h"
#include "../../EngineSetting/Optimize.h"

#include "Input.h"
#include "Renderer.h"
#include "Graphics.h"
#include "BaseSpace.h"
#include "Time.h"

//Temp include
#include "SpriteSort.h"
#include "FrameBuffersScreen.h"
#include "MSAA.h"
namespace Auto3D {

Engine::Engine(Ambient* ambient)
	:Super(ambient)
	, _isExiting(false)
{
	_ambient->RegisterSubSystem(new Renderer(_ambient));
	_ambient->RegisterSubSystem(new Graphics(_ambient));
	_ambient->RegisterSubSystem(new BaseSpace(_ambient));
	_ambient->RegisterSubSystem(new Time(_ambient));
	_ambient->RegisterSubSystem(new Input(_ambient));
}


Engine::~Engine()
{
}

void Engine::Init()
{
	GetSubSystem<Graphics>()->Init();
	GetSubSystem<BaseSpace>()->Awake();

	//Temp Sub
	_ambient->RegisterSubSystem(new SpriteSort(_ambient));

	GetSubSystem<BaseSpace>()->Start();

}
void Engine::RunFrame()
{
	auto* baseSpace = GetSubSystem<BaseSpace>();
	auto* input = GetSubSystem<Input>();
	auto* time = GetSubSystem<Time>();
	auto* renderer = GetSubSystem<Renderer>();
	time->Update();
	input->Update();
	baseSpace->Update();
	if (input->GetKeyDown(KEY_ESCAPE))
		_isExiting = true;
	Render();
	baseSpace->Finish();
	input->EndFrame();
}
void Engine::Exit() 
{
	GetSubSystem<Graphics>()->DestoryWindow();
}
void Engine::Render()
{
	auto* graphics = GetSubSystem<Graphics>();
	if (!graphics->BeginFrame())
		return;
	GetSubSystem<Renderer>()->Render();
	graphics->EndFrame();
}
}