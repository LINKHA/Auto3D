#include "Engine.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Engine::Engine()
{
	RegisterGraphicsLibrary();
	RegisterResourceLibrary();
	RegisterRendererLibrary();
}

Engine::~Engine()
{
}

void Engine::Init()
{
	_cache = new ResourceCache();
	_cache->AddResourceDir(ExecutableDir() + "Data");

	_log = new Log();
	_input = new Input();
	_profiler = new Profiler();
	_graphics = new Graphics();
	_renderer = new Renderer();

	_graphics->RenderWindow()->SetTitle("Renderer test");
	
	if (!_graphics->SetMode(RectI(0,0,800, 600), 4 , false, true))
		return;
	_graphics->RenderWindow()->SetMouseLock(true);
	_graphics->RenderWindow()->SetMouseHide(true);
	

	_renderer->SetupShadowMaps(1, 2048, FMT_D16);
}
void Engine::Start()
{

	_frameTimer = new HiresTimer();
	_profilerTimer = new Timer();
}

void Engine::Exit()
{
	LogRawString(_profilerOutput);
}

void Engine::Render()
{
	/*PROFILE(RenderScene);
	Vector<PassDesc> passes;
	passes.Push(PassDesc("opaque", SORT_STATE, true));
	passes.Push(PassDesc("alpha", SORT_BACK_TO_FRONT, true));
	_renderer->PrepareView(scene, camera, passes);

	_renderer->RenderShadowMaps();
	_graphics->ResetRenderTargets();
	_graphics->ResetViewport();
	_graphics->Clear(CLEAR_COLOR | CLEAR_DEPTH, Color::BLACK);
	_renderer->RenderBatches(passes);*/
}


bool Engine::Update()
{
	auto* input = GetSubsystem<Input>();
	auto* graphics = GetSubsystem<Graphics>();
	_frameTimer->Reset();
	if (_profilerTimer->ElapsedMSec() >= 1000)
	{
		_profilerOutput = _profiler->OutputResults();
		_profilerTimer->Reset();
		_profiler->BeginInterval();
	}

	_profiler->BeginFrame();

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
	_dt = _frameTimer->ElapsedUSec() * 0.000001f;
}

void Engine::SetPauseMinimized(bool enable)
{
	_pauseMinimized = enable;
}

}
