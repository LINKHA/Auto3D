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
	_cache = TUniquePtr<ResourceCache>(new ResourceCache());
	_log = TUniquePtr<Log>(new Log());
	_input = TUniquePtr<Input>(new Input());
	_profiler = TUniquePtr<Profiler>(new Profiler());
	_graphics = TUniquePtr<Graphics>(new Graphics());
	_renderer = TUniquePtr<Renderer>(new Renderer());
	_time = TUniquePtr<Time>(new Time());
	_registeredBox = TUniquePtr<RegisteredBox>(new RegisteredBox());
	_renderer2d = TUniquePtr<Renderer2D>(new Renderer2D());
	_physics = TUniquePtr<Physics>(new Physics());
	_fileSystem = TUniquePtr<FileSystem>(new FileSystem());
	_ui = TUniquePtr<UI>(new UI());
}

ModuleManager& ModuleManager::Get()
{
	return TSingleton<ModuleManager>::Instance();
}

void ModuleManager::RecreateCacheModule(ResourceCache* cache)
{
	if (_cache)
		_cache.Reset();

	if (cache)
		_cache = cache;
	else
		_cache = TUniquePtr<ResourceCache>(new ResourceCache());
}

void ModuleManager::RecreateGraphicsModule(Graphics* graphics)
{
	if (_graphics)
		_graphics.Reset();

	if (graphics)
		_graphics = graphics;
	else
		_graphics = TUniquePtr<Graphics>(new Graphics());
}

void ModuleManager::RecreateRendererModule(Renderer* renderer)
{
	if (_renderer)
		_renderer.Reset();

	if (renderer)
		_renderer = renderer;
	else
		_renderer = TUniquePtr<Renderer>(new Renderer());
}

void ModuleManager::RecreateInputModule(Input* input)
{
	if (_input)
		_input.Reset();

	if (input)
		_input = input;
	else
		_input = TUniquePtr<Input>(new Input());
}

void ModuleManager::RecreateLogModule(Log* log)
{
	if (_log)
		_log.Reset();

	if (log)
		_log = log;
	else
		_log = TUniquePtr<Log>(new Log());
}

void ModuleManager::RecreateProfilerModule(Profiler* profiler)
{
	if (_profiler)
		_profiler.Reset();

	if (profiler)
		_profiler = profiler;
	else
		_profiler = TUniquePtr<Profiler>(new Profiler());
}

void ModuleManager::RecreateTimeModule(Time* time)
{
	if (_time)
		_time.Reset();

	if (time)
		_time = time;
	else
		_time = TUniquePtr<Time>(new Time());
}

void ModuleManager::RecreateRegisteredBoxModule(RegisteredBox* registeredBox)
{
	if (_registeredBox)
		_registeredBox.Reset();

	if (registeredBox)
		_registeredBox = registeredBox;
	else
		_registeredBox = TUniquePtr<RegisteredBox>(new RegisteredBox());
}

void ModuleManager::RecreateRenderer2dModule(Renderer2D* renderer2d)
{
	if (_renderer2d)
		_renderer2d.Reset();

	if (renderer2d)
		_renderer2d = renderer2d;
	else
		_renderer2d = TUniquePtr<Renderer2D>(new Renderer2D());
}

void ModuleManager::RecreatePhysicsModule(Physics* physics)
{
	if (_physics)
		_physics.Reset();

	if (physics)
		_physics = physics;
	else
		_physics = TUniquePtr<Physics>(new Physics());
}

void ModuleManager::RecreateFileSystemModule(FileSystem* fileSystem)
{
	if (_fileSystem)
		_fileSystem.Reset();

	if (fileSystem)
		_fileSystem = fileSystem;
	else
		_fileSystem = TUniquePtr<FileSystem>(new FileSystem());
}

void ModuleManager::RecreateUiModule(UI* ui)
{
	if (_ui)
		_ui.Reset();

	if (ui)
		_ui = ui;
	else
		_ui = TUniquePtr<UI>(new UI());
}

void ModuleManager::RecreateAudioModule(AAudio* audio)
{
	if (_audio)
		_audio.Reset();

	if (audio)
		_audio = audio;
	else
		_audio = TUniquePtr<AAudio>(new AAudio());
}

}