#include "Engine.h"

#include "../Base/ProcessUtils.h"
#include "../Debug/DebugNew.h"
namespace Auto3D
{

Engine::Engine():
	_exiting(false),
	_initialized(false),
	_timeStep(0.0f),
	_timeStepSmoothing(2),
	_minFps(10),
#if defined(IOS) || defined(TVOS) || defined(__ANDROID__) || defined(__arm__) || defined(__aarch64__)
	_maxFps(60),
	_maxInactiveFps(10),
	_pauseMinimized(true),
#else
	_maxFps(200),
	_maxInactiveFps(60),
	_pauseMinimized(false),
#endif
	_autoExit(true)
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
	_script = new Script();
	_ui = new UI();
	_physics = new Physics();
	_fileSystem = new FileSystem();
}

Engine::~Engine()
{
}

void Engine::Init()
{
	PROFILE(EngineInit);
	// Seed by time (don't ask me why I'm using these operators, I'm scribbling)
	Time::RealTime& realTime = _time->GetRealTime();
	SetRandomSeed(realTime._year & realTime._month << realTime._day | realTime._hour * realTime._minute ^ realTime._second);

	if (!_graphics->SetMode(RectI(0, 0, 1024, 768), 4, false, true))
	{
		ErrorString("Failed to create a gutter.");
		return;
	}
	// Set default Logo
	_graphics->RenderWindow()->SetIcon(_cache->LoadResource<Image>("NewLogo.png"));

	// Init FPU state of main thread
	InitFPU();

	_frameTimer.Reset();
}
void Engine::Exit()
{
	if (_autoExit)
	{
		DoExit();
	}
}

void Engine::Render()
{
	// Check renderer render Prepare
	if (!_graphics || !_renderer || !_ui)
	{
		ErrorString("Fail to render,graphics or renderer missing!");
		return;
	}
	// Render scene
	for (auto it = _registeredBox->GetScenes().Begin(); it != _registeredBox->GetScenes().End(); it++)
	{
		_renderer->Render((*it)._first, (*it)._second);
		// Update camera aspect ratio based on window size
		(*it)._second->SetAspectRatio((float)Subsystem<Graphics>()->GetWidth() / (float)Subsystem<Graphics>()->GetHeight());
	}	

	// Render UI
	for (auto it = _registeredBox->GetCanvases().Begin(); it != _registeredBox->GetCanvases().End(); it++)
	{
		_ui->Render((*it)._first, (*it)._second);
	}
	_graphics->Present();
}


bool Engine::Update()
{
	_profiler->BeginFrame();
	_time->Update();
	_input->Update();
	//If the window is minimized do not render
	if (_graphics->RenderWindow()->IsMinimized())
		return false;
	// If the window is not initialized successfully or shutdown engine is shutdown
	if (!_graphics->IsInitialized() || _graphics->RenderWindow()->IsClose())
	{
		ShutDownEngine();
		return false;
	}
	if(Subsystem<Audio>())
		Subsystem<Audio>()->Update();

	
	return true;
}
void Engine::FrameFinish()
{
	ApplyFrameLimit();
	_profiler->EndFrame();
}

void Engine::SetMinFps(int fps)
{
	_minFps = (unsigned)Max(fps, 0);
}

void Engine::SetMaxFps(int fps)
{
	_maxFps = (unsigned)Max(fps, 0);
}

void Engine::SetMaxInactiveFps(int fps)
{
	_maxInactiveFps = (unsigned)Max(fps, 0);
}

void Engine::SetTimeStepSmoothing(int frames)
{
	_timeStepSmoothing = (unsigned)Clamp(frames, 1, 20);
}

void Engine::SetPauseMinimized(bool enable)
{
	_pauseMinimized = enable;
}

void Engine::SetNextTimeStep(float seconds)
{
	_timeStep = Max(seconds, 0.0f);
}

void Engine::SetAutoExit(bool enable)
{
	// On mobile platforms exit is mandatory if requested by the platform itself and should not be attempted to be disabled
#if defined(__ANDROID__) || defined(IOS) || defined(TVOS)
	enable = true;
#endif
	_autoExit = enable;
}

void Engine::ApplyFrameLimit()
{
	if (!_initialized)
		return;

	unsigned maxFps = _maxFps;
	auto* input = Subsystem<Input>();
	if (input)
		maxFps = Min(_maxInactiveFps, maxFps);

	long long elapsed = 0;

#ifndef __EMSCRIPTEN__
	// Perform waiting loop if maximum FPS set
#if !defined(IOS) && !defined(TVOS)
	if (maxFps)
#else
	// If on iOS/tvOS and target framerate is 60 or above, just let the animation callback handle frame timing
	// instead of waiting ourselves
	if (maxFps < 60)
#endif
	{
		PROFILE(ApplyFrameLimit);

		long long targetMax = 1000000LL / maxFps;

		for (;;)
		{
			elapsed = _frameTimer.ElapsedUSec(false);
			if (elapsed >= targetMax)
				break;

			// Sleep if 1 ms or more off the frame limiting goal
			if (targetMax - elapsed >= 1000LL)
			{
				auto sleepTime = (unsigned)((targetMax - elapsed) / 1000LL);
				Thread::Sleep(sleepTime);
			}
		}
	}
#endif

	elapsed = _frameTimer.ElapsedUSec(true);

	// If FPS lower than minimum, clamp elapsed time
	if (_minFps)
	{
		long long targetMin = 1000000LL / _minFps;
		if (elapsed > targetMin)
			elapsed = targetMin;
	}

	// Perform timestep smoothing
	_timeStep = 0.0f;
	_lastTimeSteps.Push(elapsed / 1000000.0f);
	if (_lastTimeSteps.Size() > _timeStepSmoothing)
	{
		// If the smoothing configuration was changed, ensure correct amount of samples
		_lastTimeSteps.Erase(0, _lastTimeSteps.Size() - _timeStepSmoothing);
		for (unsigned i = 0; i < _lastTimeSteps.Size(); ++i)
			_timeStep += _lastTimeSteps[i];
		_timeStep /= _lastTimeSteps.Size();
	}
	else
		_timeStep = _lastTimeSteps.Back();
}

void Engine::DoExit()
{
	auto* graphics = Subsystem<Graphics>();
	if (graphics)
		graphics->Close();

	_exiting = true;
}

}
