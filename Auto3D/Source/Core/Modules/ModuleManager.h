#pragma once
#include "Core/GameManager.h"
#include "Container/Singleton.h"
#include "Container/HashMap.h"
#include "Core/Modules/ModuleInterface.h"
#include "Core/NameTypes.h"

namespace Auto3D {

class ResourceCache;
class Graphics;
class Renderer;
class Input;
class Log;
class Profiler;
class Time;
class RegisteredBox;
class Script;
class Renderer2D;
class Physics;
class FileSystem;
class UI;
class Audio;

class AUTO_API ModuleManager : public GameManager
{
	REGISTER_OBJECT_CLASS(ModuleManager, GameManager)
	friend class Singleton<ModuleManager>;
protected:
	/// Structure
	ModuleManager();
	/// Destructor
	~ModuleManager();
public:
	/// Register all moudle library.
	void RegisterMoudleLibrary();
	/// Create all modules.
	void CreateModules();

	IModuleInterface* LoadModule(const Name& name);

	bool UnloadModule();

	/// Gets the singleton instance of the module manager.
	static ModuleManager& Get();

	/// Recreate cache moudle if not null.
	void RecreateCacheModule(ResourceCache* cache = nullptr);
	/// Recreate graphics moudle if not null.
	void RecreateGraphicsModule(Graphics* graphics = nullptr);
	/// Recreate 3d render module if not null.
	void RecreateRendererModule(Renderer* renderer = nullptr);
	/// Recreate input management events module if not null.
	void RecreateInputModule(Input* input = nullptr);
	/// Recreate Engine Log
	void RecreateLogModule(Log* log = nullptr);
	/// Recreate hierarchical performance profiler subsystem.
	void RecreateProfilerModule(Profiler* profiler = nullptr);
	/// Recreate process all engine time, calculate FPS, etc
	void RecreateTimeModule(Time* time = nullptr);
	/// Recreate the message management mechanism for the underlying interaction between the game project and the engine
	void RecreateRegisteredBoxModule(RegisteredBox* registeredBox = nullptr);
	/// Recreate 2d-related operations and rendering capabilities
	void RecreateRenderer2dModule(Renderer2D* renderer2d = nullptr);
	/// Recreate physical world and functional storage
	void RecreatePhysicsModule(Physics* physics = nullptr);
	/// Recreate an adapter system that operates on files based on the platform
	void RecreateFileSystemModule(FileSystem* fileSystem = nullptr);
	/// Recreate UI-related operations and rendering capabilities
	void RecreateUiModule(UI* ui = nullptr);
	/// Recreate component management of all audio.(Assign values when the component is first created)
	void RecreateAudioModule(Audio* audio = nullptr);

	/// Return resource loads.
	ResourceCache* CacheModule() { return _cache.Get(); }
	/// Return graphics.
	Graphics* GraphicsModule() { return _graphics.Get(); }
	/// Return 3D render.
	Renderer* RendererModule() { return _renderer.Get(); }
	/// Return input management events
	Input* InputModule() { return _input.Get(); }
	/// Engine Log
	Log* LogModule() { return _log.Get(); }
	/// Return hierarchical performance profiler subsystem.
	Profiler* ProfilerModule() { return _profiler.Get(); }
	/// Return process all engine time, calculate FPS, etc
	Time* TimeModule() { return _time.Get(); }
	/// Return the message management mechanism for the underlying interaction between the game project and the engine
	RegisteredBox* RegisteredBoxModule() { return _registeredBox.Get(); }
	/// Return 2d-related operations and rendering capabilities
	Renderer2D* Renderer2dModule() { return _renderer2d.Get(); }
	/// Return physical world and functional storage
	Physics* PhysicsModule() { return _physics.Get(); }
	/// Return an adapter system that operates on files based on the platform
	FileSystem* FileSystemModule() { return _fileSystem.Get(); }
	/// Return UI-related operations and rendering capabilities
	UI* UiModule() { return _ui.Get(); }
	/// Return component management of all audio.(Assign values when the component is first created)
	Audio* AudioModule() { return _audio.Get(); }

private:
	/// Manage the subsystem of all resource loads
	UniquePtr<ResourceCache> _cache;
	/// ADAPTS the low-level rendering interface as well as the form's rendering function
	UniquePtr<Graphics> _graphics;
	/// 3D rendering of the scene
	UniquePtr<Renderer> _renderer;
	/// User input management events
	UniquePtr<Input> _input;
	/// Engine Log
	UniquePtr<Log> _log;
	/// Hierarchical performance profiler subsystem.
	UniquePtr<Profiler> _profiler;
	/// Process all engine time, calculate FPS, etc
	UniquePtr<Time> _time;
	/// The message management mechanism for the underlying interaction between the game project and the engine
	UniquePtr<RegisteredBox> _registeredBox;
	/// 2d-related operations and rendering capabilities
	UniquePtr<Renderer2D> _renderer2d;
	/// Physical world and functional storage
	UniquePtr<Physics> _physics;
	/// An adapter system that operates on files based on the platform
	UniquePtr<FileSystem> _fileSystem;
	/// UI-related operations and rendering capabilities
	UniquePtr<UI> _ui;
	/// Component management of all audio.(Assign values when the component is first created)
	UniquePtr<Audio> _audio;

	/// Modules
	HashMap<Name, IModuleInterface> _moudles;
};

}