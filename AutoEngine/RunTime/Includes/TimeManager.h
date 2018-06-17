#ifndef TIMER_H_
#define TIMER_H_
#include "Singleton.h"
#include "GameManager.h"
#include "Math/AUMathBase.h"
#include <Windows.h>

USING_MATH
AUTO_BEGIN
/**
* @brief : Singleton class
*/


class TimeManager : public GlobalGameManager, public Singleton<TimeManager>
{
public:
	REGISTER_DERIVED_ABSTRACT_CLASS(TimeManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(TimeManager);


	struct TimeHolder
	{
		double m_CurFrameTime;
		double m_LastFrameTime;
		float m_DeltaTime;
		float m_SmoothDeltaTime;

		TimeHolder();
	};
	struct RealTime
	{
		int Year;
		int Month;
		int Day;
		int Hour;
		int Minute;
		int Second;

		RealTime();
	};
	RealTime GetRealTime();



	TimeManager();

	void SetTime(double time);
	void ResetTime();
	void SetPause(bool pause);
	void SetMaximumDeltaTime(float maxStep);
	void SetTimeScale(float scale);

	//bool StepFixedTime();
	//virtual void CheckConsistency();
	virtual void Update();

	inline double	GetCurTime() const				{ return m_ActiveTime.m_CurFrameTime; }
//	inline double	GetTimeSinceLevelLoad() const	{ return m_ActiveTime.m_CurFrameTime + m_LevelLoadOffset; }
	inline float	GetDeltaTime() const			{ return m_ActiveTime.m_DeltaTime; }
	inline float 	GetSmoothDeltaTime()  const		{ return m_ActiveTime.m_SmoothDeltaTime; }

private:
	TimeHolder  m_FixedTime;
	TimeHolder  m_DynamicTime;
	TimeHolder  m_ActiveTime;

	RealTime	m_RealTime;

	bool		is_Pause;


//	double      m_LevelLoadOffset;
	float		m_MaximumTimestep;
	float		m_TimeSpeedScale;//1.0 is real time 0.5 is low time(range 0,100)

	bool		m_FirstFrame;// Don't do anything to delta time the first frame!

};
AUTO_END
#endif //!TIMER_H_

