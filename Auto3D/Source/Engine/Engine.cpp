#include "Engine.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Engine::Engine()
{
	RegisterGraphicsLibrary();
	RegisterResourceLibrary();
	RegisterRendererLibrary();

	_cache = new ResourceCache();
	_cache->AddResourceDir(ExecutableDir() + "Data");

	_log = new Log();
	_input = new Input();
	_profiler = new Profiler();
	_graphics = new Graphics();
	_renderer = new Renderer();
	_time = new Time();
	_sceneSystem = new SceneSystem();
	_audio = new Audio();
	_script = new Script();
	_ui = new UI();
	_physics = new Physics();

}

Engine::~Engine()
{
}

void Engine::Init()
{
	
	// Seed by time (don't ask me why I'm using these operators, I'm scribbling)
	Time::RealTime& realTime = _time->GetRealTime();
	SetRandomSeed(realTime._year & realTime._month << realTime._day | realTime._hour * realTime._minute ^ realTime._second);

	_graphics->RenderWindow()->SetTitle("Renderer test");
	
	if (!_graphics->SetMode(RectI(0, 0, 800, 600), 4, false, true))
		return;
	_graphics->RenderWindow()->SetMouseLock(true);
	_graphics->RenderWindow()->SetMouseHide(true);
	

	_renderer->SetupShadowMaps(1, 2048, ImageFormat::D16);
}
void Engine::Exit()
{

}

void Engine::Render()
{
	for (auto it = _sceneSystem->GetScenes().Begin(); it != _sceneSystem->GetScenes().End(); it++)
	{
		_renderer->Render((*it).first, (*it).second);
	}	
}


bool Engine::Update()
{
	auto* input = Subsystem<Input>();
	auto* graphics = Subsystem<Graphics>();

	_profiler->BeginFrame();

	_time->Update();
	_input->Update();

	if (input->IsKeyPress(27))
		graphics->Close();

	
	if (!graphics->IsInitialized())
	{
		ShutDownEngine();
		return false;
	}

	return true;
}
void Engine::FrameFinish()
{
	_graphics->Present();

	_profiler->EndFrame();
}

void Engine::SetPauseMinimized(bool enable)
{
	_pauseMinimized = enable;
}

}
