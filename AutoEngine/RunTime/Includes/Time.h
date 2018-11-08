#pragma once
#include <Windows.h>
#include "GameManager.h"
#include "AutoSDL2.h"

namespace Auto3D {


/**
* Save time information 
*/
struct TimeHolder
{
	/// the reality of the game from now on
	double curFrameTime{};
	/// the scene begins with the actual use of the previous frame
	double lastFrameTime{};
	/// time used in the previous frame
	float deltaTime{};
	/// smoothing delta time weight
	float smoothingWeight{};
	/// average deltaTime
	float smoothDeltaTime{};
};

/**
* Save world time information
*/
struct RealTime
{
	int year{};
	int month{};
	int day{};
	int hour{};
	int minute{};
	int second{};
};

struct TimerMSG
{
	int ID;
	STRING name;
	int index;
};

/**
* SubSystem class for time
*/
class Time : public GlobalGameManager
{

	REGISTER_DERIVED_CLASS(Time, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Time);
	typedef void(__cdecl* TimerCallback) ();
public:
	explicit Time(Ambient* ambient);
	/**
	* @brief : Reset time
	*/
	void ResetTime();
	/**
	* @brief : SubSystem to engine update
	*/
	void Update();
	/**
	* @brief : Calculate the average delta time  to smoothDelta
	*/
	void CalcSmoothDeltaTime(TimeHolder& time);
	/**
	* @brief : Sleep for a number of milliseconds.
	*/
	void Sleep(unsigned millisecond);
	/**
	* @brief : Set current frame time
	*/
	void SetTime(double time);
	/**
	* @brief : Set pause key enable
	*/
	void SetPause(bool pause);
	/**
	* @brief : Set maximum delta time
	*/
	void SetMaximumDeltaTime(float maxStep);
	/**
	* @brief : Set time scale range(0~100)
	*/
	void SetTimeScale(float scale);
	/**
	* @brief : Get real time now
	* @return : RealTime
	*/
	RealTime& GetRealTime();
	/**
	* @brief : Return the time from the first frame of seconds.(affected by suspension))
	*/
	inline double GetCurTime() const { return _activeTime.curFrameTime; }
	/**
	* @brief : Returns the time when the previous frame was run 
	*/
	inline float GetDeltaTime() const { return _activeTime.deltaTime; }
	/**
	* @brief : Returns smooth delta time
	*/
	inline float GetSmoothDeltaTime() const { return _activeTime.smoothDeltaTime; }
	/**
	* @brief : Return number for engine frame
	*/
	inline unsigned GetFrameCount() const { return _frameCount; }
	/**
	* @brief : Return the time from the first frame of seconds. (Not affected by suspension)
	*/
	double GetTimeSinceStartup() const;
	/**
	* @brief : Return current frames per second.
	*/
	float Time::GetFramesPerSecond() const;
	/**
	* @brief : One shot timer
	*/
	void OneShotTimer(TimerCallback callback, int msTime);
	/**
	* @brief : Run every msTime for a total of count 
				(no infinite loop, use Timer if necessary)
	*/
	void ShotTimer(TimerCallback callback, int msTime, int count = 1);
private:
	/**
	* @brief : Run every msTime for a total of count
	*			(no infinite loop, use Timer if necessary)
	*/
	void timerCount(TimerCallback callback, int msTime, int count);
private:
	/// dynamic time holder
	TimeHolder _dynamicTime;
	/// active time holder
	TimeHolder _activeTime;
	/// world real time 
	RealTime _realTime;
	/// maximum time step if more than will clamp delta time
	float _maximumTimestep;
	///1.0 is real time 0.5 is low time(range 0,100)
	float _timeSpeedScale;
	/// frame number
	unsigned _frameCount{};
	/// zero time
	double _zeroTime{};
	/// don't do anything to delta time the first frame
	bool _isFirstFrameAfterReset;
	/// pause the first frame
	bool _isFirstFrameAfterPause;
	/// is pause timer
	bool _isTimerPause;
};
}


