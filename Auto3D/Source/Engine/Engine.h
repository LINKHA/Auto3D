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
	AutoPtr<ResourceCache> _cache;
	AutoPtr<Graphics> _graphics;
	AutoPtr<Renderer> _renderer;
	AutoPtr<Input> _input;
	AutoPtr<Log> _log;
	AutoPtr<Profiler> _profiler;
	AutoPtr<Time> _time;
	AutoPtr<RegisteredBox> _registeredBox;
	AutoPtr<Script> _script;
	AutoPtr<UI> _ui;
	AutoPtr<Physics> _physics;
	AutoPtr<FileSystem> _fileSystem;
	//This subsystem is implemented in the Audio component, the first one created
	//AutoPtr<Audio> _audio;


	/// Is exiting
	bool _exiting{};
	/// Is initialized
	bool _initialized{};
	/// Pause when _minimized flag.
	bool _pauseMinimized{};
};


}