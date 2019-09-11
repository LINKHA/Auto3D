#pragma once
#include "../Base/GameManager.h"
#include "../Container/Singleton.h"

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

	/// Gets the singleton instance of the module manager.
	static ModuleManager& Get();

	/// Manage the subsystem of all resource loads
	SharedPtr<ResourceCache> _cache;
	/// ADAPTS the low-level rendering interface as well as the form's rendering function
	SharedPtr<Graphics> _graphics;
	/// 3D rendering of the scene
	SharedPtr<Renderer> _renderer;
	/// User input management events
	SharedPtr<Input> _input;
	/// Engine Log
	SharedPtr<Log> _log;
#ifdef AUTO_PROFILING
	/// Hierarchical performance profiler subsystem.
	SharedPtr<Profiler> _profiler;
#endif
	/// Process all engine time, calculate FPS, etc
	SharedPtr<Time> _time;
	/// The message management mechanism for the underlying interaction between the game project and the engine
	SharedPtr<RegisteredBox> _registeredBox;
	/// 2d-related operations and rendering capabilities
	SharedPtr<Renderer2D> _renderer2d;
	/// Physical world and functional storage
	SharedPtr<Physics> _physics;
	/// An adapter system that operates on files based on the platform
	SharedPtr<FileSystem> _fileSystem;
	/// UI-related operations and rendering capabilities
	SharedPtr<UI> _ui;
	/// Component management of all audio.(Assign values when the component is first created)
	SharedPtr<Audio> _audio;
};

}