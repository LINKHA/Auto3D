#include "Engine.h"
#include "Input.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Audio.h"
#include "Time.h"
#include "Script.h"
#include "ResourceSystem.h"
#include "Scene.h"
#include "IO.h"
#include "UI.h"
#include "Behavior.h"
#include "FileSystem.h"

namespace Auto3D {

Engine::Engine(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	_ambient->RegisterSubSystem(MakeShared<Renderer>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Graphics>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Time>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Input>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Script>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<ResourceSystem>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Scene>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<IO>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Audio>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<UI>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Behavior>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<FileSystem>(_ambient));
}

Engine::~Engine()
{

	_ambient->RemoveSubSystem<Graphics>();
	_ambient->RemoveSubSystem<Time>();
	_ambient->RemoveSubSystem<Input>();
	_ambient->RemoveSubSystem<Script>();
	_ambient->RemoveSubSystem<ResourceSystem>();
	_ambient->RemoveSubSystem<Scene>();
	_ambient->RemoveSubSystem<IO>();
	_ambient->RemoveSubSystem<Audio>();
	_ambient->RemoveSubSystem<UI>();
	_ambient->RemoveSubSystem<Behavior>();
	_ambient->RemoveSubSystem<FileSystem>();
	_ambient->RemoveSubSystem<Renderer>();
}

void Engine::Init()
{
	GetSubSystem<IO>()->GetEngineInfo()->state = EngineState::Initing;
	GetSubSystem<ResourceSystem>()->Init();
	GetSubSystem<Audio>()->Init();
	GetSubSystem<Graphics>()->Init();
	GetSubSystem<Behavior>()->Awake();

	Print("Temp comment tag");
	//GetSubSystem<Renderer>()->Init();
	GetSubSystem<Behavior>()->Start();

	Print("Temp comment tag");
	//GetSubSystem<Renderer>()->ReadyToRender();

}

void Engine::Exit() 
{
	GetSubSystem<IO>()->GetEngineInfo()->state = EngineState::Exiting;
	GetSubSystem<Graphics>()->DestoryWindow();
#if AUTO_DIRECT_X
	GetSubSystem<Graphics>()->ReleaseAPI();
#endif
}
void Engine::Render()
{
	auto graphics = GetSubSystem<Graphics>();
	GetSubSystem<IO>()->GetEngineInfo()->state = EngineState::Rendering;

	if (!graphics->BeginFrame())
		return;
	Print("Temp comment tag");
	//GetSubSystem<Renderer>()->Render();
	graphics->EndFrame();
}
void Engine::Update()
{
	auto engineInfo = GetSubSystem<IO>()->GetEngineInfo();
	auto timeSub = GetSubSystem<Time>();

	engineInfo->state = EngineState::Updateing;
	timeSub->Update();

	engineInfo->fps = timeSub->GetFramesPerSecond();
	engineInfo->frameCount = timeSub->GetFrameCount();

	GetSubSystem<Input>()->Update();
	GetSubSystem<Behavior>()->Update();

}
void Engine::FrameFinish()
{
	GetSubSystem<Behavior>()->Finish();
	GetSubSystem<Input>()->EndFrame();
}


}