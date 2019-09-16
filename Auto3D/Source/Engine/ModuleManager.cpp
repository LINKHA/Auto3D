#include "ModuleManager.h"

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
#include "../UI/Canvas.h"
#include "../Scene/Scene.h"
#include "../Auto2D/Scene2D.h"
#include "../Base/ProcessUtils.h"
#include "../Debug/DebugNew.h"

namespace Auto3D {

ModuleManager::ModuleManager() :
	_cache(nullptr),
	_graphics(nullptr),
	_renderer(nullptr),
	_input(nullptr),
	_log(nullptr),
#ifdef AUTO_PROFILING
	_profiler(nullptr),
#endif
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
	_cache = new ResourceCache();
	_cache->AddResourceDir(ExecutableDir() + "Data");

	_log = new Log();
	_input = new Input();
#ifdef AUTO_PROFILING
	_profiler = new Profiler();
#endif
	_graphics = new Graphics();
	_renderer = new Renderer();
	_time = new Time();
	_registeredBox = new RegisteredBox();
	_renderer2d = new Renderer2D();
	_physics = new Physics();
	_fileSystem = new FileSystem();
	_ui = new UI();
}

ModuleManager& ModuleManager::Get()
{
	return Singleton<ModuleManager>::Instance();
}


}