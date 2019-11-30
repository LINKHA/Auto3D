#pragma once
#include "Core/GameManager.h"
#include "Container/Singleton.h"
#include "Container/HashMap.h"
#include "Core/Modules/ModuleInterface.h"
#include "Core/NameTypes.h"

namespace Auto3D {

class AResourceCache;
class AGraphics;
class ARenderer;
class AInput;
class ALog;
class AProfiler;
class ATime;
class ARegisteredBox;
class AScript;
class Renderer2D;
class APhysics;
class AFileSystem;
class AUIModule;
class AAudio;

class AUTO_API GModuleManager //: public AGameManager
{
	//REGISTER_OBJECT_CLASS(GModuleManager, AGameManager)
	friend class TSingleton<GModuleManager>;
protected:
	/// Structure
	GModuleManager();
	/// Destructor
	~GModuleManager();
public:
	/// Register all moudle library.
	void RegisterMoudleLibrary();
	/// Create all modules.
	void CreateModules();

	/// Gets the singleton instance of the module manager.
	static GModuleManager& Get();

	/// Recreate cache moudle if not null.
	void RecreateCacheModule(AResourceCache* cache = nullptr);
	/// Recreate graphics moudle if not null.
	void RecreateGraphicsModule(AGraphics* graphics = nullptr);
	/// Recreate 3d render module if not null.
	void RecreateRendererModule(ARenderer* renderer = nullptr);
	/// Recreate input management events module if not null.
	void RecreateInputModule(AInput* input = nullptr);
	/// Recreate AEngine ALog
	void RecreateLogModule(ALog* log = nullptr);
	/// Recreate hierarchical performance profiler subsystem.
	void RecreateProfilerModule(AProfiler* profiler = nullptr);
	/// Recreate process all engine time, calculate FPS, etc
	void RecreateTimeModule(ATime* time = nullptr);
	/// Recreate the message management mechanism for the underlying interaction between the game project and the engine
	void RecreateRegisteredBoxModule(ARegisteredBox* registeredBox = nullptr);
	/// Recreate 2d-related operations and rendering capabilities
	void RecreateRenderer2dModule(Renderer2D* renderer2d = nullptr);
	/// Recreate physical world and functional storage
	void RecreatePhysicsModule(APhysics* physics = nullptr);
	/// Recreate an adapter system that operates on files based on the platform
	void RecreateFileSystemModule(AFileSystem* fileSystem = nullptr);
	/// Recreate AUIModule-related operations and rendering capabilities
	void RecreateUiModule(AUIModule* ui = nullptr);
	/// Recreate component management of all audio.(Assign values when the component is first created)
	void RecreateAudioModule(AAudio* audio = nullptr);

	/// Return resource loads.
	AResourceCache* CacheModule() { return _cache.Get(); }
	/// Return graphics.
	AGraphics* GraphicsModule() { return _graphics.Get(); }
	/// Return 3D render.
	ARenderer* RendererModule() { return _renderer.Get(); }
	/// Return input management events
	AInput* InputModule() { return _input.Get(); }
	/// AEngine ALog
	ALog* LogModule() { return _log.Get(); }
	/// Return hierarchical performance profiler subsystem.
	AProfiler* ProfilerModule() { return _profiler.Get(); }
	/// Return process all engine time, calculate FPS, etc
	ATime* TimeModule() { return _time.Get(); }
	/// Return the message management mechanism for the underlying interaction between the game project and the engine
	ARegisteredBox* RegisteredBoxModule() { return _registeredBox.Get(); }
	/// Return 2d-related operations and rendering capabilities
	Renderer2D* Renderer2dModule() { return _renderer2d.Get(); }
	/// Return physical world and functional storage
	APhysics* PhysicsModule() { return _physics.Get(); }
	/// Return an adapter system that operates on files based on the platform
	AFileSystem* FileSystemModule() { return _fileSystem.Get(); }
	/// Return AUIModule-related operations and rendering capabilities
	AUIModule* UiModule() { return _ui.Get(); }
	/// Return component management of all audio.(Assign values when the component is first created)
	AAudio* AudioModule() { return _audio.Get(); }
private:
	/// Manage the subsystem of all resource loads
	TUniquePtr<AResourceCache> _cache;
	/// ADAPTS the low-level rendering interface as well as the form's rendering function
	TUniquePtr<AGraphics> _graphics;
	/// 3D rendering of the scene
	TUniquePtr<ARenderer> _renderer;
	/// User input management events
	TUniquePtr<AInput> _input;
	/// AEngine ALog
	TUniquePtr<ALog> _log;
	/// Hierarchical performance profiler subsystem.
	TUniquePtr<AProfiler> _profiler;
	/// Process all engine time, calculate FPS, etc
	TUniquePtr<ATime> _time;
	/// The message management mechanism for the underlying interaction between the game project and the engine
	TUniquePtr<ARegisteredBox> _registeredBox;
	/// 2d-related operations and rendering capabilities
	TUniquePtr<Renderer2D> _renderer2d;
	/// Physical world and functional storage
	TUniquePtr<APhysics> _physics;
	/// An adapter system that operates on files based on the platform
	TUniquePtr<AFileSystem> _fileSystem;
	/// AUIModule-related operations and rendering capabilities
	TUniquePtr<AUIModule> _ui;
	/// Component management of all audio.(Assign values when the component is first created)
	TUniquePtr<AAudio> _audio;
};

}