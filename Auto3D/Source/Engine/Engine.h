#include "../Object/Object.h"
#include "../Auto.h"

namespace Auto3D
{

class SceneMsg
{

};

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
	/// Return whether to pause update events and audio when _minimized.
	bool GetPauseMinimized() const { return _pauseMinimized; }
	/// Set whether to pause update events and audio when _minimized
	void SetPauseMinimized(bool enable);

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


	/// Is exiting
	bool _exiting{};
	/// Is initialized
	bool _initialized{};
	/// Pause when _minimized flag.
	bool _pauseMinimized{};
};


}