#include "ModuleManager.h"

#include "Audio/Audio.h"
#include "Resource/ResourceCache.h"
#include "Graphics/Graphics.h"
#include "Renderer/Renderer.h"
#include "Platform/Input.h"
#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Time/Time.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Script/Script.h"
#include "Physics/Physics.h"
#include "IO/FileSystem.h"
#include "UI/UI.h"
#include "Auto2D/Renderer2D.h"
#include "Scene/Scene.h"
#include "Auto2D/Scene2D.h"
#include "Core/ProcessUtils.h"

#include "Debug/DebugNew.h"

namespace Auto3D {

IMPLEMENT_SINGLETON(GModuleManager)

GModuleManager::GModuleManager() :
	_cache(nullptr),
	_graphics(nullptr),
	_renderer(nullptr),
	_input(nullptr),
	_log(nullptr),
	_profiler(nullptr),
	_time(nullptr),
	_registeredBox(nullptr),
	_renderer2d(nullptr),
	_physics(nullptr),
	_fileSystem(nullptr),
	_ui(nullptr),
	_audio(nullptr)
{}

GModuleManager::~GModuleManager()
{}

void GModuleManager::RegisterMoudleLibrary()
{
	RegisterGraphicsLibrary();
	RegisterResourceLibrary();
	RegisterRendererLibrary();
	RegisterRenderer2DLibrary();
	RegisterAudioLibrary();
	RegisterUILibrary();
	RegisterPhysicsLibrary();
}

void GModuleManager::CreateModules()
{
	_cache = TUniquePtr<FResourceModule>(new FResourceModule());
	_log = TUniquePtr<FLogModule>(new FLogModule());
	_input = TUniquePtr<FInputModule>(new FInputModule());
	_profiler = TUniquePtr<FProfilerModule>(new FProfilerModule());
	_graphics = TUniquePtr<FGraphicsModule>(new FGraphicsModule());
	_renderer = TUniquePtr<FRendererModule>(new FRendererModule());
	_time = TUniquePtr<FTimeModule>(new FTimeModule());
	_registeredBox = TUniquePtr<FRegisteredBoxModule>(new FRegisteredBoxModule());
	_renderer2d = TUniquePtr<FRenderer2DModule>(new FRenderer2DModule());
	_physics = TUniquePtr<FPhysicsModule>(new FPhysicsModule());
	_fileSystem = TUniquePtr<FFileModule>(new FFileModule());
	_ui = TUniquePtr<FUIModule>(new FUIModule());
}

void GModuleManager::RecreateCacheModule(FResourceModule* cache)
{
	if (_cache)
		_cache.Reset();

	if (cache)
		_cache = cache;
	else
		_cache = TUniquePtr<FResourceModule>(new FResourceModule());
}

void GModuleManager::RecreateGraphicsModule(FGraphicsModule* graphics)
{
	if (_graphics)
		_graphics.Reset();

	if (graphics)
		_graphics = graphics;
	else
		_graphics = TUniquePtr<FGraphicsModule>(new FGraphicsModule());
}

void GModuleManager::RecreateRendererModule(FRendererModule* renderer)
{
	if (_renderer)
		_renderer.Reset();

	if (renderer)
		_renderer = renderer;
	else
		_renderer = TUniquePtr<FRendererModule>(new FRendererModule());
}

void GModuleManager::RecreateInputModule(FInputModule* input)
{
	if (_input)
		_input.Reset();

	if (input)
		_input = input;
	else
		_input = TUniquePtr<FInputModule>(new FInputModule());
}

void GModuleManager::RecreateLogModule(FLogModule* log)
{
	if (_log)
		_log.Reset();

	if (log)
		_log = log;
	else
		_log = TUniquePtr<FLogModule>(new FLogModule());
}

void GModuleManager::RecreateProfilerModule(FProfilerModule* profiler)
{
	if (_profiler)
		_profiler.Reset();

	if (profiler)
		_profiler = profiler;
	else
		_profiler = TUniquePtr<FProfilerModule>(new FProfilerModule());
}

void GModuleManager::RecreateTimeModule(FTimeModule* time)
{
	if (_time)
		_time.Reset();

	if (time)
		_time = time;
	else
		_time = TUniquePtr<FTimeModule>(new FTimeModule());
}

void GModuleManager::RecreateRegisteredBoxModule(FRegisteredBoxModule* registeredBox)
{
	if (_registeredBox)
		_registeredBox.Reset();

	if (registeredBox)
		_registeredBox = registeredBox;
	else
		_registeredBox = TUniquePtr<FRegisteredBoxModule>(new FRegisteredBoxModule());
}

void GModuleManager::RecreateRenderer2dModule(FRenderer2DModule* renderer2d)
{
	if (_renderer2d)
		_renderer2d.Reset();

	if (renderer2d)
		_renderer2d = renderer2d;
	else
		_renderer2d = TUniquePtr<FRenderer2DModule>(new FRenderer2DModule());
}

void GModuleManager::RecreatePhysicsModule(FPhysicsModule* physics)
{
	if (_physics)
		_physics.Reset();

	if (physics)
		_physics = physics;
	else
		_physics = TUniquePtr<FPhysicsModule>(new FPhysicsModule());
}

void GModuleManager::RecreateFileSystemModule(FFileModule* fileSystem)
{
	if (_fileSystem)
		_fileSystem.Reset();

	if (fileSystem)
		_fileSystem = fileSystem;
	else
		_fileSystem = TUniquePtr<FFileModule>(new FFileModule());
}

void GModuleManager::RecreateUiModule(FUIModule* ui)
{
	if (_ui)
		_ui.Reset();

	if (ui)
		_ui = ui;
	else
		_ui = TUniquePtr<FUIModule>(new FUIModule());
}

void GModuleManager::RecreateAudioModule(FAudioModule* audio)
{
	if (_audio)
		_audio.Reset();

	if (audio)
		_audio = audio;
	else
		_audio = TUniquePtr<FAudioModule>(new FAudioModule());
}

}