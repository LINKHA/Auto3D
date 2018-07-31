#pragma once
#include "GameManager.h"
#include "Math/MathBase.h"
#include <Windows.h>


namespace Auto3D {
/**
* @brief : SubSystem class
*/
class Time : public GlobalGameManager
{
public:
	REGISTER_DERIVED_CLASS(Time, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Time);


	struct TimeHolder
	{
		double curFrameTime;
		double lastFrameTime;
		float deltaTime;
		float smoothDeltaTime;

		TimeHolder();
	};
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
	RealTime GetRealTime();



	explicit Time(Ambient* ambient);

	void SetTime(double time);
	void ResetTime();
	void SetPause(bool pause);
	void SetMaximumDeltaTime(float maxStep);
	void SetTimeScale(float scale);

	//bool StepFixedTime();
	//virtual void CheckConsistency();
	virtual void Update();

	inline double	GetCurTime() const				{ return _activeTime.curFrameTime; }
//	inline double	GetTimeSinceLevelLoad() const	{ return _activeTime.curFrameTime + m_LevelLoadOffset; }
	inline float	GetDeltaTime() const			{ return _activeTime.deltaTime; }
	inline float 	GetSmoothDeltaTime()  const		{ return _activeTime.smoothDeltaTime; }

private:
	TimeHolder  _fixedTime;
	TimeHolder  _dynamicTime;
	TimeHolder  _activeTime;

	RealTime	_realTime;

	bool		_isPause;


//	double      m_LevelLoadOffset;
	float		_maximumTimestep;
	float		_timeSpeedScale;//1.0 is real time 0.5 is low time(range 0,100)

	bool		_firstFrame;// Don't do anything to delta time the first frame!

};
}


