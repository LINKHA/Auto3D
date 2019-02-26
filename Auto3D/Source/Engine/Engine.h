#include "../Object/Object.h"
#include "../Auto.h"

namespace Auto3D
{

class AUTO_API Engine : public Object
{
	REGISTER_OBJECT_CLASS(Engine, Object)
public:
	/**
	* @brief : Construct
	*/
	Engine();

	~Engine();
	/**
	* @brief : Init engine, Register some SubSystem
	*/
	void Init();

	void Start();
	/**
	* @brief : Engine normal exit
	*/
	void Exit();
	/**
	* @brief : Shut down engine
	*/
	void ShutDownEngine() { _exiting = true; }
	/**
	* @brief : Engine exit flag
	*/
	bool IsExiting()const { return _exiting; }
	/**
	* @brief : Render geometry
	*/
	void Render();
	/**
	* @brief : Sub system update data,  If pause when _minimized -mode return false
	*/
	bool Update();
	/**
	* @brief : Frame finish
	*/
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

	AutoPtr<HiresTimer> _frameTimer;
	AutoPtr<Timer> _profilerTimer;
	float _dt = 0.0f;
	String _profilerOutput;

	/// Is exiting
	bool _exiting{};
	/// Is initialized
	bool _initialized{};
	/// Pause when _minimized flag.
	bool _pauseMinimized;
};


}