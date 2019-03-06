#include "Engine.h"

#include "../Debug/DebugNew.h"
namespace Auto3D
{

Engine::Engine()
{
	RegisterGraphicsLibrary();
	RegisterResourceLibrary();
	RegisterRendererLibrary();
	RegisterUILibrary();
	RegisterAudioLibrary();

	_cache = new ResourceCache();
	_cache->AddResourceDir(ExecutableDir() + "Data");

	_log = new Log();
	_input = new Input();
	_profiler = new Profiler();
	_graphics = new Graphics();
	_renderer = new Renderer();
	_time = new Time();
	_registeredBox = new RegisteredBox();
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

	if (!_graphics->SetMode(RectI(0, 0, 1024, 768), 4, false, true))
	{
		ErrorString("Failed to create a gutter.");
		return;
	}
}
void Engine::Exit()
{

}

void Engine::Render()
{
	// Check renderer render Prepare
	if (!_graphics || !_renderer || !_ui)
	{
		ErrorString("Fail to render,graphics or renderer missing!");
		return;
	}
	// If the scene is not created, clear the previous frame's texture here
	if (!_registeredBox->GetScenes().Size())
	{
		_graphics->ResetRenderTargets();
		_graphics->ResetViewport();
		_graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL, Color::BLACK);
	}
	// Render scene
	for (auto it = _registeredBox->GetScenes().Begin(); it != _registeredBox->GetScenes().End(); it++)
	{
		_renderer->Render((*it).first, (*it).second);
		// Update camera aspect ratio based on window size
		(*it).second->SetAspectRatio((float)Subsystem<Graphics>()->GetWidth() / (float)Subsystem<Graphics>()->GetHeight());
	}	

	// Render UI
	for (auto it = _registeredBox->GetCanvases().Begin(); it != _registeredBox->GetCanvases().End(); it++)
	{
		_ui->Render((*it).first, (*it).second);
		// Update camera aspect ratio based on window size
		//(*it).second->SetAspectRatio((float)Subsystem<Graphics>()->GetWidth() / (float)Subsystem<Graphics>()->GetHeight());
	}
	
	_graphics->Present();
}


bool Engine::Update()
{
	auto* input = Subsystem<Input>();
	auto* graphics = Subsystem<Graphics>();

	_profiler->BeginFrame();

	_time->Update();
	_input->Update();
	_audio->Update();

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
	_profiler->EndFrame();
}

void Engine::SetPauseMinimized(bool enable)
{
	_pauseMinimized = enable;
}

}
