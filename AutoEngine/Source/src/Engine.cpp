#include "Engine.h"
#include "Input.h"
#include "Renderer.h"
#include "Graphics.h"
#include "Audio.h"
#include "Time.h"
#include "ResourceSystem.h"
#include "IO.h"
#include "UI.h"
#include "FileSystem.h"
#include "EngineParameter.h"
#include "Batch.h"


namespace Auto3D {

Engine::Engine(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _pauseMinimized(false)
{
	_ambient->RegisterSubSystem(MakeShared<Renderer>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Graphics>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Time>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Input>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<ResourceSystem>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<IO>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<Audio>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<UI>(_ambient));
	_ambient->RegisterSubSystem(MakeShared<FileSystem>(_ambient));
}

Engine::~Engine()
{

	_ambient->RemoveSubSystem<Graphics>();
	_ambient->RemoveSubSystem<Time>();
	_ambient->RemoveSubSystem<Input>();
	_ambient->RemoveSubSystem<ResourceSystem>();
	_ambient->RemoveSubSystem<IO>();
	_ambient->RemoveSubSystem<Audio>();
	_ambient->RemoveSubSystem<UI>();
	_ambient->RemoveSubSystem<FileSystem>();
	_ambient->RemoveSubSystem<Renderer>();
}

void Engine::Init()
{
	auto renderer = GetSubSystem<Renderer>();
	GetSubSystem<IO>()->GetEngineInfo()->state = EngineState::Initing;
	GetSubSystem<ResourceSystem>()->Init();
	GetSubSystem<Graphics>()->Init();
	renderer->SetupShadowMaps(1, 2048, ImageFormat::D16);

//	SharedPtr<Scene> scene = MakeShared<Scene>(_ambient);
	//scene->CreateChild<Octree>();
	//Camera* camera = scene->CreateChild<Camera>();
	//camera->SetPosition(Vector3(0.0f, 20.0f, -75.0f));
	//camera->SetAmbientColor(Color(0.1f, 0.1f, 0.1f));


	_isInitialized = true;
}

void Engine::Exit() 
{
	GetSubSystem<IO>()->GetEngineInfo()->state = EngineState::Exiting;
	GetSubSystem<Graphics>()->DestoryWindow();
	GetSubSystem<Graphics>()->ReleaseAPI();
}
void Engine::Render()
{
	auto graphics = GetSubSystem<Graphics>();
	GetSubSystem<IO>()->GetEngineInfo()->state = EngineState::Rendering;

	if (!graphics->BeginFrame())
		return;
	auto renderer = GetSubSystem<Renderer>();
	//VECTOR<PassDesc> passes;
	//passes.push_back(PassDesc("opaque", SORT_STATE, true));
	//passes.push_back(PassDesc("alpha", SORT_BACK_TO_FRONT, true));
	//renderer->PrepareView(scene, camera, passes);

	graphics->EndFrame();
}
bool Engine::Update()
{
	auto engineInfo = GetSubSystem<IO>()->GetEngineInfo();
	auto timeSub = GetSubSystem<Time>();
	auto input = GetSubSystem<Input>();

	engineInfo->state = EngineState::Updateing;
	timeSub->Update();

	engineInfo->fps = timeSub->GetFramesPerSecond();
	engineInfo->frameCount = timeSub->GetFrameCount();

	input->Update();
	// close
	if (input->GetAppQuit())
		ShutDownEngine();
	// If pause when minimized -mode is in use, stop updates and audio as necessary
	if (input->GetMinimized())
		return false;

	return true;
}
void Engine::FrameFinish()
{
	GetSubSystem<Input>()->EndFrame();
}

void Engine::SetPauseMinimized(bool enable)
{
	_pauseMinimized = enable;
}

}