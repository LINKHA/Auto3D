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
	_cache = TUniquePtr<AResourceCache>(new AResourceCache());
	_log = TUniquePtr<ALog>(new ALog());
	_input = TUniquePtr<AInput>(new AInput());
	_profiler = TUniquePtr<AProfiler>(new AProfiler());
	_graphics = TUniquePtr<AGraphics>(new AGraphics());
	_renderer = TUniquePtr<ARenderer>(new ARenderer());
	_time = TUniquePtr<ATime>(new ATime());
	_registeredBox = TUniquePtr<ARegisteredBox>(new ARegisteredBox());
	_renderer2d = TUniquePtr<ARenderer2D>(new ARenderer2D());
	_physics = TUniquePtr<APhysics>(new APhysics());
	_fileSystem = TUniquePtr<AFileSystem>(new AFileSystem());
	_ui = TUniquePtr<AUIModule>(new AUIModule());
}

GModuleManager& GModuleManager::Get()
{
	return TSingleton<GModuleManager>::Instance();
}

void GModuleManager::RecreateCacheModule(AResourceCache* cache)
{
	if (_cache)
		_cache.Reset();

	if (cache)
		_cache = cache;
	else
		_cache = TUniquePtr<AResourceCache>(new AResourceCache());
}

void GModuleManager::RecreateGraphicsModule(AGraphics* graphics)
{
	if (_graphics)
		_graphics.Reset();

	if (graphics)
		_graphics = graphics;
	else
		_graphics = TUniquePtr<AGraphics>(new AGraphics());
}

void GModuleManager::RecreateRendererModule(ARenderer* renderer)
{
	if (_renderer)
		_renderer.Reset();

	if (renderer)
		_renderer = renderer;
	else
		_renderer = TUniquePtr<ARenderer>(new ARenderer());
}

void GModuleManager::RecreateInputModule(AInput* input)
{
	if (_input)
		_input.Reset();

	if (input)
		_input = input;
	else
		_input = TUniquePtr<AInput>(new AInput());
}

void GModuleManager::RecreateLogModule(ALog* log)
{
	if (_log)
		_log.Reset();

	if (log)
		_log = log;
	else
		_log = TUniquePtr<ALog>(new ALog());
}

void GModuleManager::RecreateProfilerModule(AProfiler* profiler)
{
	if (_profiler)
		_profiler.Reset();

	if (profiler)
		_profiler = profiler;
	else
		_profiler = TUniquePtr<AProfiler>(new AProfiler());
}

void GModuleManager::RecreateTimeModule(ATime* time)
{
	if (_time)
		_time.Reset();

	if (time)
		_time = time;
	else
		_time = TUniquePtr<ATime>(new ATime());
}

void GModuleManager::RecreateRegisteredBoxModule(ARegisteredBox* registeredBox)
{
	if (_registeredBox)
		_registeredBox.Reset();

	if (registeredBox)
		_registeredBox = registeredBox;
	else
		_registeredBox = TUniquePtr<ARegisteredBox>(new ARegisteredBox());
}

void GModuleManager::RecreateRenderer2dModule(ARenderer2D* renderer2d)
{
	if (_renderer2d)
		_renderer2d.Reset();

	if (renderer2d)
		_renderer2d = renderer2d;
	else
		_renderer2d = TUniquePtr<ARenderer2D>(new ARenderer2D());
}

void GModuleManager::RecreatePhysicsModule(APhysics* physics)
{
	if (_physics)
		_physics.Reset();

	if (physics)
		_physics = physics;
	else
		_physics = TUniquePtr<APhysics>(new APhysics());
}

void GModuleManager::RecreateFileSystemModule(AFileSystem* fileSystem)
{
	if (_fileSystem)
		_fileSystem.Reset();

	if (fileSystem)
		_fileSystem = fileSystem;
	else
		_fileSystem = TUniquePtr<AFileSystem>(new AFileSystem());
}

void GModuleManager::RecreateUiModule(AUIModule* ui)
{
	if (_ui)
		_ui.Reset();

	if (ui)
		_ui = ui;
	else
		_ui = TUniquePtr<AUIModule>(new AUIModule());
}

void GModuleManager::RecreateAudioModule(AAudio* audio)
{
	if (_audio)
		_audio.Reset();

	if (audio)
		_audio = audio;
	else
		_audio = TUniquePtr<AAudio>(new AAudio());
}

}