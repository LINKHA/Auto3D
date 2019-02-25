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
	_graphics->RenderWindow()->SetMouseVisible(false);
	if (!_graphics->SetMode(IntVector2(800, 600), false, true))
		return;

	_renderer->SetupShadowMaps(1, 2048, FMT_D16);

	//SubscribeToEvent(_renderer->RenderWindow()->closeRequestEvent, &RendererTest::HandleCloseRequest);
}
void Engine::Start()
{

	frameTimer = new HiresTimer();
	profilerTimer = new Timer();
}

void Engine::Exit()
{
	LOGRAW(profilerOutput);
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
	auto* input = Subsystem<Input>();
	auto* graphics = Subsystem<Graphics>();
	frameTimer->Reset();
	if (profilerTimer->ElapsedMSec() >= 1000)
	{
		profilerOutput = _profiler->OutputResults();
		profilerTimer->Reset();
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
	dt = frameTimer->ElapsedUSec() * 0.000001f;
}

void Engine::SetPauseMinimized(bool enable)
{
	_pauseMinimized = enable;
}

}
