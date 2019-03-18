#include "../Object/Object.h"
#include "../Auto.h"

namespace Auto3D
{

class AUTO_API Engine : public Object
{
	REGISTER_OBJECT_CLASS(Engine, Object)
public:
	/// Construct
	Engine();

	~Engine();
	/// Init engine, Register some SubSystem
	void Init();
	/// Engine normal exit
	void Exit();
	/// Shut down engine
	void ShutDownEngine() { _exiting = true; }
	/// Engine exit flag
	bool IsExiting()const { return _exiting; }
	/// Render geometry
	void Render();
	/// Sub system update data,  If pause when _minimized -mode return false
	bool Update();
	/// Frame finish
	void FrameFinish();

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
	/// Get the timestep for the next frame and sleep for frame limiting if necessary.
	void ApplyFrameLimit();
private:
	UniquePtr<ResourceCache> _cache;
	UniquePtr<Graphics> _graphics;
	UniquePtr<Renderer> _renderer;
	UniquePtr<Input> _input;
	UniquePtr<Log> _log;
	UniquePtr<Profiler> _profiler;
	UniquePtr<Time> _time;
	UniquePtr<RegisteredBox> _registeredBox;
	UniquePtr<Script> _script;
	UniquePtr<UI> _ui;
	UniquePtr<Physics> _physics;
	UniquePtr<FileSystem> _fileSystem;
	//This subsystem is implemented in the Audio component, the first one created
	//UniquePtr<Audio> _audio;

	/// Frame update timer.
	HiresTimer _frameTimer;
	/// Is exiting
	bool _exiting;
	/// Is initialized
	bool _initialized;
	/// Pause when _minimized flag.
	bool _pauseMinimized;
	/// Previous timesteps for smoothing.
	Vector<float> _lastTimeSteps;
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
};


}