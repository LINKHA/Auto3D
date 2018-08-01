#include "Time.h"
#include "SDL2/SDL.h"
namespace Auto3D {
SYSTEMTIME  sysTime;
Time::TimeHolder::TimeHolder()
	: curFrameTime(0)
	, lastFrameTime(0)
	, deltaTime(0)
	, smoothDeltaTime(0)
{}

Time::RealTime::RealTime()
{
	GetLocalTime(&sysTime);
	year = sysTime.wYear;
	month = sysTime.wMonth;
	day = sysTime.wDay;
	hour = sysTime.wHour;
	minute = sysTime.wMinute;
	second = sysTime.wSecond;
}


Time::RealTime Time::GetRealTime()
{
	GetLocalTime(&sysTime);
	_realTime.year = sysTime.wYear;
	_realTime.month = sysTime.wMonth;
	_realTime.day = sysTime.wDay;
	_realTime.hour = sysTime.wHour;
	_realTime.minute = sysTime.wMinute;
	_realTime.second = sysTime.wSecond;
	return _realTime;
}

/////////////////////////////////////////////////////////////////////////////////////////////
Time::Time(Ambient* ambient)
	: Super(ambient)
{
	_realTime = RealTime();

	_dynamicTime.curFrameTime = 0.0f;
	_dynamicTime.lastFrameTime = 0.0f;
	_dynamicTime.deltaTime = 0.0f;
	_dynamicTime.smoothDeltaTime = 0.0f;
	_activeTime = _dynamicTime;
	_isPause = false;

}


void Time::SetTime(double time)
{
	_dynamicTime.lastFrameTime = _dynamicTime.curFrameTime;
	_dynamicTime.curFrameTime = time;
	_dynamicTime.deltaTime = _dynamicTime.curFrameTime - _dynamicTime.lastFrameTime;
	_activeTime = _dynamicTime;
}
void Time::ResetTime()
{
	_dynamicTime.curFrameTime = 0.0f;
	_dynamicTime.lastFrameTime = 0.0f;
	_dynamicTime.deltaTime = 0.0f;
	_dynamicTime.smoothDeltaTime = 0.0f;

//	m_FixedTime.m_CurFrameTime = 0.0f;
//	m_FixedTime.m_LastFrameTime = 0.0f;
	_activeTime = _dynamicTime;


//	m_LevelLoadOffset = 0.0f;
}

void Time::SetPause(bool pause)
{
	_isPause = pause;
}
void Time::SetMaximumDeltaTime(float maxStep)
{
	_maximumTimestep = max<float>(maxStep, _fixedTime.deltaTime);
}

void Time::SetTimeScale(float scale)
{
	bool is_OutRange = scale <= 100 && scale >= 0.0f;
	if (is_OutRange)
		_timeSpeedScale = scale;
	else
		ErrorString("time speed scale is out of range.Range(0~100).");

}

void Time::Update()
{
	if (_firstFrame)
	{
		_firstFrame = false;
		return;
	}
	double time = SDL_GetTicks();
	SetTime(time/1000);
}


Time::~Time(){}
}

