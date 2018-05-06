#include "TimeManager.h"
#include "GrFacade.h"
AUTO_BEGIN

template<> TimeManager* Singleton<TimeManager>::m_instance = nullptr;
SYSTEMTIME  m_sysTime;


TimeManager::TimeHolder::TimeHolder()
	: m_CurFrameTime(0)
	, m_LastFrameTime(0)
	, m_DeltaTime(0)
	, m_SmoothDeltaTime(0)
{}

TimeManager::RealTime::RealTime()
{
	GetLocalTime(&m_sysTime);
	Year = m_sysTime.wYear;
	Month = m_sysTime.wMonth;
	Day = m_sysTime.wDay;
	Hour = m_sysTime.wHour;
	Minute = m_sysTime.wMinute;
	Second = m_sysTime.wSecond;
}


TimeManager::RealTime TimeManager::GetRealTime()
{
	GetLocalTime(&m_sysTime);
	m_RealTime.Year = m_sysTime.wYear;
	m_RealTime.Month = m_sysTime.wMonth;
	m_RealTime.Day = m_sysTime.wDay;
	m_RealTime.Hour = m_sysTime.wHour;
	m_RealTime.Minute = m_sysTime.wMinute;
	m_RealTime.Second = m_sysTime.wSecond;
	return m_RealTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////
TimeManager::TimeManager()
	: Super()
{

	m_RealTime = RealTime();

	m_DynamicTime.m_CurFrameTime = 0.0f;
	m_DynamicTime.m_LastFrameTime = 0.0f;
	m_DynamicTime.m_DeltaTime = 0.0f;
	m_DynamicTime.m_SmoothDeltaTime = 0.0f;

	//m_FixedTime.m_CurFrameTime = 0.0f;
	//m_FixedTime.m_LastFrameTime = 0.0f;
	m_ActiveTime = m_DynamicTime;

	is_Pause = false;

//	m_LevelLoadOffset = 0.0f;
}


void TimeManager::SetTime(double time)
{
	m_DynamicTime.m_LastFrameTime = m_DynamicTime.m_CurFrameTime;
	m_DynamicTime.m_CurFrameTime = time;
	m_DynamicTime.m_DeltaTime = m_DynamicTime.m_CurFrameTime - m_DynamicTime.m_LastFrameTime;
	m_ActiveTime = m_DynamicTime;
}
void TimeManager::ResetTime()
{
	m_DynamicTime.m_CurFrameTime = 0.0f;
	m_DynamicTime.m_LastFrameTime = 0.0f;
	m_DynamicTime.m_DeltaTime = 0.0f;
	m_DynamicTime.m_SmoothDeltaTime = 0.0f;

//	m_FixedTime.m_CurFrameTime = 0.0f;
//	m_FixedTime.m_LastFrameTime = 0.0f;
	m_ActiveTime = m_DynamicTime;


//	m_LevelLoadOffset = 0.0f;
}

void TimeManager::SetPause(bool pause)
{
	is_Pause = pause;
}
void TimeManager::SetMaximumDeltaTime(float maxStep)
{
	m_MaximumTimestep = max<float>(maxStep, m_FixedTime.m_DeltaTime);
}

void TimeManager::SetTimeScale(float scale)
{
	bool is_OutRange = scale <= 100 && scale >= 0.0f;
	if (is_OutRange)
		m_TimeSpeedScale = scale;
	else
		ErrorString("time speed scale is out of range.Range(0~100).");

}

void TimeManager::Update()
{
	if (m_FirstFrame)
	{
		m_FirstFrame = false;
		return;
	}
	double time = GrGetTime();
	SetTime(time);
}


TimeManager::~TimeManager(){}
AUTO_END

