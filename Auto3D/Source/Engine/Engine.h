#pragma once
#include "Core/Object/Object.h"
#include "Time/Time.h"

namespace Auto3D
{

class GModuleManager;
class FResourceModule;
class FGraphicsModule;
class FForwardShadingRenderer;
class FInputModule;
class FLogModule;
class FProfilerModule;
class FTimeModule;
class GWorldContext;
class AScript;
class FRenderer2DModule;
class FPhysicsModule;
class FFileModule;
class FUIModule;

class AUTO_API AEngine : public AObject
{
	DECLARE_CLASS(AEngine, AObject)
public:
	/// Construct
	AEngine();
	/// Cestructor
	~AEngine();
	/// Init engine, Register some SubSystem
	bool Init();
	/// AEngine normal exit
	void Exit();
	/// Render geometry
	void Render();
	/// Sub system update data,  If pause when _minimized -mode return false
	bool Update();
	/// Frame finish
	void FrameFinish();

	/// Shut down engine
	void ShutDownEngine() { _exiting = true; }
	/// AEngine exit flag
	bool IsExiting()const { return _exiting; }
	/// Set minimum frames per second. If FPS goes lower than this, time will appear to slow down.
	void SetMinFps(int fps);
	/// Set maximum frames per second. The engine will sleep if FPS is higher than this.
	void SetMaxFps(int fps);
	/// Set maximum frames per second when the application does not have input focus.
	void SetMaxInactiveFps(int fps);
	/// Set how many frames to average for timestep smoothing. Default is 2. 1 disables smoothing.
	void SetTimeStepSmoothing(int frames);
	/// Set whether to pause update events and audio when minimized.
	void SetPauseMinimized(bool enable);
	/// Override timestep of the next frame. Should be called in between RunFrame() calls.
	void SetNextTimeStep(float seconds);
	/// Set whether to exit automatically on exit request (window close button.)
	void SetAutoExit(bool enable);
	/// Return whether to pause update events and audio when _minimized.
	bool GetPauseMinimized() const { return _pauseMinimized; }
	/// Get timestep of the next frame. Updated by ApplyFrameLimit().
	float GetNextTimeStep() const { return _timeStep; }
	/// Return the minimum frames per second.
	int GetMinFps() const { return _minFps; }
	/// Return the maximum frames per second.
	int GetMaxFps() const { return _maxFps; }
	/// Return the maximum frames per second when the application does not have input focus.
	int GetMaxInactiveFps() const { return _maxInactiveFps; }
	/// Return how many frames to average for timestep smoothing.
	int GetTimeStepSmoothing() const { return _timeStepSmoothing; }
	/// Return whether to exit automatically on exit request.
	bool GetAutoExit() const { return _autoExit; }
	/// Get the timestep for the next frame and sleep for frame limiting if necessary.
	void ApplyFrameLimit();
private:
	/// Actually perform the exit actions.
	void DoExit();

	/// Frame update timer.
	FHiresTimer _frameTimer;
	/// Is exiting
	bool _exiting;
	/// Is initialized
	bool _initialized;
	/// Pause when _minimized flag.
	bool _pauseMinimized;
	/// Auto-exit flag.
	bool _autoExit;
	/// Previous timesteps for smoothing.
	TVector<float> _lastTimeSteps;
	/// Next frame timestep in seconds.
	float _timeStep;
	/// How many frames to average for the smoothed timestep.
	unsigned _timeStepSmoothing;
	/// Maximum FPS
	unsigned _maxFps;
	/// Minimum FPS
	unsigned _minFps;
	/// Maximum frames per second when the application does not have input focus.
	unsigned _maxInactiveFps;


	/// Manage the subsystem of all resource loads
	FResourceModule* _cache;
	/// ADAPTS the low-level rendering interface as well as the form's rendering function
	FGraphicsModule* _graphics;
	/// 3D rendering of the scene
	FForwardShadingRenderer* _renderer;
	/// User input management events
	FInputModule* _input;
	/// AEngine ALog
	FLogModule* _log;
	/// Hierarchical performance profiler subsystem.
	FProfilerModule* _profiler;
	/// Process all engine time, calculate FPS, etc
	FTimeModule* _time;
	/// 2d-related operations and rendering capabilities
	FRenderer2DModule* _renderer2d;
	/// Physical world and functional storage
	FPhysicsModule* _physics;
	/// An adapter system that operates on files based on the platform
	FFileModule* _fileSystem;
	/// AUIModule-related operations and rendering capabilities
	FUIModule* _ui;
};


}