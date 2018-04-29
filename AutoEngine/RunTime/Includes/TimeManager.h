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
	




	TimeManager();

	void SetTime(double time);
	void ResetTime();
	void SetPause(bool pause);
	bool StepFixedTime();
	RealTime GetRealTime();


	//virtual void CheckConsistency();
	//virtual void Update();


	inline double	GetCurTime() const				{ return m_ActiveTime.m_CurFrameTime; }
	inline double	GetTimeSinceLevelLoad() const	{ return m_ActiveTime.m_CurFrameTime + m_LevelLoadOffset; }
	inline float	GetDeltaTime() const			{ return m_ActiveTime.m_DeltaTime; }
	inline float 	GetSmoothDeltaTime()  const		{ return m_ActiveTime.m_SmoothDeltaTime; }

private:
	TimeHolder  m_FixedTime;
	TimeHolder  m_DynamicTime;
	TimeHolder  m_ActiveTime;
	RealTime	m_RealTime;

	bool		is_Pause;
	double      m_LevelLoadOffset;
};
AUTO_END
#endif //!TIMER_H_

