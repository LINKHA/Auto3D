#include "Engine.h"

#include "../Platform/Window.h"
#include "../Thread/Thread.h"

#include "../Audio/Audio.h"
#include "../Resource/ResourceCache.h"
#include "../Graphics/Graphics.h"
#include "../Renderer/Renderer.h"
#include "../Platform/Input.h"
#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "../Time/Time.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Script/Script.h"
#include "../Physics/Physics.h"
#include "../IO/FileSystem.h"
#include "../UI/UI.h"
#include "../Auto2D/Renderer2D.h"
#include "../Scene/Scene.h"
#include "../Auto2D/Scene2D.h"
#include "../Base/ProcessUtils.h"
#include "../Debug/DebugNew.h"

#include "ModuleManager.h"


namespace Auto3D
{

Engine::Engine() :
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
	ModuleManager& moduleManager = ModuleManager::Get();
	moduleManager.RegisterMoudleLibrary();
	moduleManager.CreateModules();

	// Assign a value to this class
	_cache = moduleManager.CacheModule();
	_graphics = moduleManager.GraphicsModule();
	_renderer = moduleManager.RendererModule();
	_input = moduleManager.InputModule();
	_log = moduleManager.LogModule();
	_profiler = moduleManager.ProfilerModule();
	_time = moduleManager.TimeModule();
	_registeredBox = moduleManager.RegisteredBoxModule();
	_renderer2d = moduleManager.Renderer2dModule();
	_physics = moduleManager.PhysicsModule();
	_fileSystem = moduleManager.FileSystemModule();
	_ui = moduleManager.UiModule();
}

Engine::~Engine()
{
}

bool Engine::Init()
{
	if (_initialized)
		return true;
	PROFILE(EngineInit);

	// Set random seeds based on time
	Time::RealTime& realTime = _time->GetRealTime();

	SetRandomSeed(((unsigned)(realTime._day & 0xff) << 24) |
		((unsigned)(realTime._hour & 0xff) << 16) |
		((unsigned)(realTime._minute & 0xff) << 8) |
		((unsigned)(realTime._second & 0xff)));

	WindowModeDesc& windowModeDesc = _graphics->RenderWindow()->ModeDesc();
	if (!_graphics->SetMode(windowModeDesc))
	{
		ErrorString("Failed to create a gutter.");
		return false;
	}

	// Set default Logo
	_graphics->RenderWindow()->SetIcon(_cache->LoadResource<Image>("Texture/NewLogo.png"));

	if (!_graphics->RenderWindow())
		return false;

#ifdef AUTO_OPENGL
	if (!_ui->SetMode(_graphics->RenderWindow(), _graphics->RenderContext()))
#else
	if (!_ui->SetMode(_graphics->RenderWindow()))
#endif
	{
		ErrorString("Failed to create a ui.");
		return false;
	}

	// Init FPU state of main thread
	InitFPU();

	_frameTimer.Reset();

	_initialized = true;

	return true;
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
	PROFILE(Render);
	// Check renderer render Prepare
	if (!_graphics || !_renderer || !_renderer2d)
	{
		ErrorString("Fail to render,graphics or renderer missing!");
		return;
	}
	_graphics->Prepare();
	// Render scene
	{
		PROFILE(RenderScene);

		Scene* scene = _registeredBox->GetActiveScene();
		if (scene && scene->IsEnabled())
		{
			Vector<Camera*>& cameras = scene->GetAllCamera();
			for (auto cameraIt = cameras.Begin(); cameraIt != cameras.End(); ++cameraIt)
			{
				Camera* camera = *cameraIt;
				_renderer->Render(scene, camera);
				// Update camera aspect ratio based on window size
				camera->SetAspectRatio((float)_graphics->GetWidth() / (float)_graphics->GetHeight());
			}
		}
	}
	// Render Renderer2D
	{
		PROFILE(RenderScene2D);

		Scene2D* scene2d = _registeredBox->GetActiveScene2D();
		if (scene2d && scene2d->IsEnabled())
		{
			Vector<Camera2D*>& cameras = scene2d->GetAllCamera();
			for (auto cameraIt = cameras.Begin(); cameraIt != cameras.End(); ++cameraIt)
			{
				Camera2D* camera = *cameraIt;
				_renderer2d->Render(scene2d, camera);
			}
		}
	}
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
	if (!_graphics->IsInitialized() || _graphics->RenderWindow()->IsCloseing())
	{
		ShutDownEngine();
		return false;
	}
	if (Audio* audio = ModuleManager::Get().AudioModule())
		audio->Update();

	_physics->Update();
	_ui->BeginUI();

	return true;
}
void Engine::FrameFinish()
{
	//Present ui and graphics
	{
		PROFILE(RenderPresent);
		_ui->Present();
		_graphics->Present();
	}

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

	if (_input)
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
	if (_graphics)
		_graphics->Close();
	_exiting = true;
	auto* profiler = ModuleManager::Get().ProfilerModule();
	LogString(profiler->OutputResults());
}

}
