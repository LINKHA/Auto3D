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

ModuleManager::ModuleManager() :
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

ModuleManager::~ModuleManager()
{}

void ModuleManager::RegisterMoudleLibrary()
{
	RegisterGraphicsLibrary();
	RegisterResourceLibrary();
	RegisterRendererLibrary();
	RegisterRenderer2DLibrary();
	RegisterAudioLibrary();
	RegisterUILibrary();
	RegisterPhysicsLibrary();
}

void ModuleManager::CreateModules()
{
	_cache = UniquePtr<ResourceCache>(new ResourceCache());
	_log = UniquePtr<Log>(new Log());
	_input = UniquePtr<Input>(new Input());
	_profiler = UniquePtr<Profiler>(new Profiler());
	_graphics = UniquePtr<Graphics>(new Graphics());
	_renderer = UniquePtr<Renderer>(new Renderer());
	_time = UniquePtr<Time>(new Time());
	_registeredBox = UniquePtr<RegisteredBox>(new RegisteredBox());
	_renderer2d = UniquePtr<Renderer2D>(new Renderer2D());
	_physics = UniquePtr<Physics>(new Physics());
	_fileSystem = UniquePtr<FileSystem>(new FileSystem());
	_ui = UniquePtr<UI>(new UI());
}

IModuleInterface* ModuleManager::LoadModule(const Name& name)
{

	return nullptr;
}

bool ModuleManager::UnloadModule()
{
	return true;
}

ModuleManager& ModuleManager::Get()
{
	return Singleton<ModuleManager>::Instance();
}

void ModuleManager::RecreateCacheModule(ResourceCache* cache)
{
	if (_cache)
		_cache.Reset();

	if (cache)
		_cache = cache;
	else
		_cache = UniquePtr<ResourceCache>(new ResourceCache());
}

void ModuleManager::RecreateGraphicsModule(Graphics* graphics)
{
	if (_graphics)
		_graphics.Reset();

	if (graphics)
		_graphics = graphics;
	else
		_graphics = UniquePtr<Graphics>(new Graphics());
}

void ModuleManager::RecreateRendererModule(Renderer* renderer)
{
	if (_renderer)
		_renderer.Reset();

	if (renderer)
		_renderer = renderer;
	else
		_renderer = UniquePtr<Renderer>(new Renderer());
}

void ModuleManager::RecreateInputModule(Input* input)
{
	if (_input)
		_input.Reset();

	if (input)
		_input = input;
	else
		_input = UniquePtr<Input>(new Input());
}

void ModuleManager::RecreateLogModule(Log* log)
{
	if (_log)
		_log.Reset();

	if (log)
		_log = log;
	else
		_log = UniquePtr<Log>(new Log());
}

void ModuleManager::RecreateProfilerModule(Profiler* profiler)
{
	if (_profiler)
		_profiler.Reset();

	if (profiler)
		_profiler = profiler;
	else
		_profiler = UniquePtr<Profiler>(new Profiler());
}

void ModuleManager::RecreateTimeModule(Time* time)
{
	if (_time)
		_time.Reset();

	if (time)
		_time = time;
	else
		_time = UniquePtr<Time>(new Time());
}

void ModuleManager::RecreateRegisteredBoxModule(RegisteredBox* registeredBox)
{
	if (_registeredBox)
		_registeredBox.Reset();

	if (registeredBox)
		_registeredBox = registeredBox;
	else
		_registeredBox = UniquePtr<RegisteredBox>(new RegisteredBox());
}

void ModuleManager::RecreateRenderer2dModule(Renderer2D* renderer2d)
{
	if (_renderer2d)
		_renderer2d.Reset();

	if (renderer2d)
		_renderer2d = renderer2d;
	else
		_renderer2d = UniquePtr<Renderer2D>(new Renderer2D());
}

void ModuleManager::RecreatePhysicsModule(Physics* physics)
{
	if (_physics)
		_physics.Reset();

	if (physics)
		_physics = physics;
	else
		_physics = UniquePtr<Physics>(new Physics());
}

void ModuleManager::RecreateFileSystemModule(FileSystem* fileSystem)
{
	if (_fileSystem)
		_fileSystem.Reset();

	if (fileSystem)
		_fileSystem = fileSystem;
	else
		_fileSystem = UniquePtr<FileSystem>(new FileSystem());
}

void ModuleManager::RecreateUiModule(UI* ui)
{
	if (_ui)
		_ui.Reset();

	if (ui)
		_ui = ui;
	else
		_ui = UniquePtr<UI>(new UI());
}

void ModuleManager::RecreateAudioModule(Audio* audio)
{
	if (_audio)
		_audio.Reset();

	if (audio)
		_audio = audio;
	else
		_audio = UniquePtr<Audio>(new Audio());
}


}