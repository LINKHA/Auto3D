#pragma once
#include "Container/Singleton.h"
#include "Container/HashMap.h"
#include "Core/Modules/ModuleInterface.h"
#include "Core/NameTypes.h"

namespace Auto3D {

class FResourceModule;
class FGraphicsModule;
class FForwardShadingRenderer;
class FInputModule;
class FLogModule;
class FProfilerModule;
class FTimeModule;
class AScript;
class FRenderer2DModule;
class FPhysicsModule;
class FFileModule;
class FUIModule;
class FAudioModule;
class FSerializationModule;

class AUTO_API GModuleManager 
{
	REGISTER_SINGLETON(GModuleManager)
protected:
	/// Structure
	GModuleManager();
	/// Destructor
	~GModuleManager();
public:

	/// Create all modules.
	void CreateModules();

	/// Recreate cache moudle if not null.
	void RecreateCacheModule(FResourceModule* cache = nullptr);
	/// Recreate graphics moudle if not null.
	void RecreateGraphicsModule(FGraphicsModule* graphics = nullptr);
	/// Recreate 3d render module if not null.
	void RecreateRendererModule(FForwardShadingRenderer* renderer = nullptr);
	/// Recreate input management events module if not null.
	void RecreateInputModule(FInputModule* input = nullptr);
	/// Recreate AEngine ALog
	void RecreateLogModule(FLogModule* log = nullptr);
	/// Recreate hierarchical performance profiler subsystem.
	void RecreateProfilerModule(FProfilerModule* profiler = nullptr);
	/// Recreate process all engine time, calculate FPS, etc
	void RecreateTimeModule(FTimeModule* time = nullptr);
	/// Recreate 2d-related operations and rendering capabilities
	void RecreateRenderer2dModule(FRenderer2DModule* renderer2d = nullptr);
	/// Recreate physical world and functional storage
	void RecreatePhysicsModule(FPhysicsModule* physics = nullptr);
	/// Recreate an adapter system that operates on files based on the platform
	void RecreateFileSystemModule(FFileModule* fileSystem = nullptr);
	/// Recreate AUIModule-related operations and rendering capabilities
	void RecreateUiModule(FUIModule* ui = nullptr);
	/// Recreate component management of all audio.(Assign values when the component is first created)
	void RecreateAudioModule(FAudioModule* audio = nullptr);
	/// Recreate serialization module
	void RecreateSerializationModule(FSerializationModule* serialization = nullptr);

	/// Return resource loads.
	FResourceModule* CacheModule() { return _cache.Get(); }
	/// Return graphics.
	FGraphicsModule* GraphicsModule() { return _graphics.Get(); }
	/// Return 3D render.
	FForwardShadingRenderer* RendererModule() { return _renderer.Get(); }
	/// Return input management events
	FInputModule* InputModule() { return _input.Get(); }
	/// AEngine ALog
	FLogModule* LogModule() { return _log.Get(); }
	/// Return hierarchical performance profiler subsystem.
	FProfilerModule* ProfilerModule() { return _profiler.Get(); }
	/// Return process all engine time, calculate FPS, etc
	FTimeModule* TimeModule() { return _time.Get(); }
	/// Return 2d-related operations and rendering capabilities
	FRenderer2DModule* Renderer2dModule() { return _renderer2d.Get(); }
	/// Return physical world and functional storage
	FPhysicsModule* PhysicsModule() { return _physics.Get(); }
	/// Return an adapter system that operates on files based on the platform
	FFileModule* FileSystemModule() { return _fileSystem.Get(); }
	/// Return AUIModule-related operations and rendering capabilities
	FUIModule* UiModule() { return _ui.Get(); }
	/// Return component management of all audio.(Assign values when the component is first created)
	FAudioModule* AudioModule() { return _audio.Get(); }
	/// Return serialization module.
	FSerializationModule* SerializationModule() { return _serialization.Get(); }
private:
	/// Manage the subsystem of all resource loads
	TUniquePtr<FResourceModule> _cache;
	/// ADAPTS the low-level rendering interface as well as the form's rendering function
	TUniquePtr<FGraphicsModule> _graphics;
	/// 3D rendering of the scene
	TUniquePtr<FForwardShadingRenderer> _renderer;
	/// User input management events
	TUniquePtr<FInputModule> _input;
	/// AEngine ALog
	TUniquePtr<FLogModule> _log;
	/// Hierarchical performance profiler subsystem.
	TUniquePtr<FProfilerModule> _profiler;
	/// Process all engine time, calculate FPS, etc
	TUniquePtr<FTimeModule> _time;
	/// 2d-related operations and rendering capabilities
	TUniquePtr<FRenderer2DModule> _renderer2d;
	/// Physical world and functional storage
	TUniquePtr<FPhysicsModule> _physics;
	/// An adapter system that operates on files based on the platform
	TUniquePtr<FFileModule> _fileSystem;
	/// AUIModule-related operations and rendering capabilities
	TUniquePtr<FUIModule> _ui;
	/// Component management of all audio.(Assign values when the component is first created)
	TUniquePtr<FAudioModule> _audio;
	/// Serialization module.
	TUniquePtr<FSerializationModule> _serialization;
};

}