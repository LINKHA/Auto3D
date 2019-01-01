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
	_ambient->RemoveSubSystem<Renderer>();
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
}

void Engine::Init()
{
	GetSubSystem<ResourceSystem>()->Init();
	GetSubSystem<Audio>()->Init();
	GetSubSystem<Graphics>()->Init();
	GetSubSystem<Behavior>()->Awake();
	GetSubSystem<Renderer>()->Init();
	GetSubSystem<Behavior>()->Start();
	GetSubSystem<Renderer>()->ReadyToRender();

}
void Engine::RunFrame()
{
	update();
	render();
	frameFinish();
}
void Engine::Exit() 
{
	GetSubSystem<Graphics>()->DestoryWindow();
}
void Engine::render()
{
	auto graphics = GetSubSystem<Graphics>();

	if (!graphics->BeginFrame())
		return;
	GetSubSystem<Renderer>()->Render();
	graphics->EndFrame();
}
void Engine::update()
{
	auto input = GetSubSystem<Input>();
	GetSubSystem<Time>()->Update();
	input->Update();
	GetSubSystem<Behavior>()->Update();
	if (input->GetKeyDown(KEY_ESCAPE))
		_isExiting = true;
}
void Engine::frameFinish()
{
	GetSubSystem<Behavior>()->Finish();
	GetSubSystem<Input>()->EndFrame();
}


}