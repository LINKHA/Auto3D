#pragma once
#include <Windows.h>
#include "GameManager.h"

namespace Auto3D {
/**
* @brief : Save time information 
*/
struct TimeHolder
{
	double curFrameTime;
	double lastFrameTime;
	float deltaTime;
	float smoothDeltaTime;

	TimeHolder()
		: curFrameTime(0)
		, lastFrameTime(0)
		, deltaTime(0)
		, smoothDeltaTime(0)
	{}
};
/**
* @brief : Save world time information
*/
struct RealTime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	RealTime();
};
/**
* @brief : SubSystem class for time
*/
class Time : public GlobalGameManager
{

	REGISTER_DERIVED_CLASS(Time, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Time);
public:
	explicit Time(Ambient* ambient);
	/**
	* @brief : Get real time now
	* @return : RealTime
	*/
	RealTime GetRealTime();
	/**
	* @brief : Set current frame time
	*/
	void SetTime(double time);
	/**
	* @brief : Set pause key enable
	*/
	void SetPause(bool pause);
	void SetMaximumDeltaTime(float maxStep);
	void SetTimeScale(float scale);

	inline double GetCurTime() const { return _activeTime.curFrameTime; }
	inline float GetDeltaTime() const { return _activeTime.deltaTime; }
	inline float GetSmoothDeltaTime() const { return _activeTime.smoothDeltaTime; }
	
	void ResetTime();
	virtual void Update();
private:
	TimeHolder  _fixedTime;
	TimeHolder  _dynamicTime;
	TimeHolder  _activeTime;
	RealTime	_realTime;
	bool		_isPause;
	float		_maximumTimestep;
	float		_timeSpeedScale;//1.0 is real time 0.5 is low time(range 0,100)
	bool		_firstFrame;// Don't do anything to delta time the first frame!
};
}


