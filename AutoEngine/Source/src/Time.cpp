#include "Time.h"
#include "AuMath.h"
#include "AutoSDL.h"
#include "AutoThread.h"
#include "NewDef.h"

namespace Auto3D {

SYSTEMTIME  sysTime;

const float MAXIMUM_DELTA_TIME = 1.0f / 3.0f;
const float MINIMUM_DELTA_TIME = 0.00001f;
const float START_UP_DELTA_TIME = 0.02f;
const float NEW_DELTA_TIME_WEIGHT = 0.2f; // for smoothing


Time::Time(SharedPtr<Ambient> ambient)
	: Super(ambient)
{
	_timeSpeedScale = 1.0f;
	_maximumTimestep = MAXIMUM_DELTA_TIME;

	ResetTime();

}

Time::~Time() {}

void Time::Update()
{
	_frameCount++;
	if (!_frameCount)
		_frameCount++;

	if (_isFirstFrameAfterReset)
	{
		_isFirstFrameAfterReset = false;
		return;
	}
	// When coming out of a pause / startup / level load we don't want to have a spike in delta time.
	// So just default to START_UP_DELTA_TIME.
	if (_isFirstFrameAfterPause)
	{
		_isFirstFrameAfterPause = false;
		SetTime(_dynamicTime.curFrameTime + START_UP_DELTA_TIME * _timeSpeedScale);
		// This is not a real delta time so don't include in smoothed time
		_activeTime.smoothingWeight = 0.0f;
		_dynamicTime.smoothingWeight = 0.0f;
		return;
	}

	double time = GetTimeSinceStartup() - _zeroTime;

	// clamp the delta time in case a frame takes too long.
	if (time - _dynamicTime.curFrameTime > _maximumTimestep)
	{
		SetTime(_dynamicTime.curFrameTime + _maximumTimestep * _timeSpeedScale);
		return;
	}
	// clamp the delta time in case a frame goes to fast! (prevent delta time being zero)
	if (time - _dynamicTime.curFrameTime < MINIMUM_DELTA_TIME)
	{
		SetTime(_dynamicTime.curFrameTime + MINIMUM_DELTA_TIME * _timeSpeedScale);
		return;
	}
	// handle time pause
	if (_isTimerPause)
	{
		SetTime(_dynamicTime.curFrameTime);
		// This is not a real delta time so don't include in smoothed time
		_activeTime.smoothingWeight = 0.0f;
		_dynamicTime.smoothingWeight = 0.0f;
		return;
	}
	// handle time scale
	if (!CompareApproximately(_timeSpeedScale, 1.0f))
	{
		float deltaTime = time - _dynamicTime.curFrameTime;
		SetTime(_dynamicTime.curFrameTime + deltaTime * _timeSpeedScale);
		return;
	}

	_dynamicTime.lastFrameTime = _dynamicTime.curFrameTime;
	_dynamicTime.curFrameTime = time;
	_dynamicTime.deltaTime = _dynamicTime.curFrameTime - _dynamicTime.lastFrameTime;
	CalcSmoothDeltaTime(_dynamicTime);

	_activeTime = _dynamicTime;

}

void Time::ResetTime()
{
	_dynamicTime.curFrameTime = 0.0f;
	_dynamicTime.lastFrameTime = 0.0f;
	_dynamicTime.deltaTime = 0.0f;
	_dynamicTime.smoothDeltaTime = 0.0f;
	_activeTime = _dynamicTime;
	_isFirstFrameAfterReset = true;
	_isFirstFrameAfterPause = true;
	_isTimerPause = false;
	_frameCount = 0;
	_zeroTime = GetTimeSinceStartup();
}

void Time::CalcSmoothDeltaTime(TimeHolder& time)
{
	// If existing weight is zero, don't take existing value into account
	time.smoothingWeight *= (1.0f - NEW_DELTA_TIME_WEIGHT);
	time.smoothingWeight += NEW_DELTA_TIME_WEIGHT;
	// As confidence in smoothed value increases the divisor goes towards 1
	float normalized = NEW_DELTA_TIME_WEIGHT / time.smoothingWeight;
	time.smoothDeltaTime = lerp(time.smoothDeltaTime, time.deltaTime, normalized);
}

RealTime& Time::GetRealTime()
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

void Time::SetTime(double time)
{
	_dynamicTime.lastFrameTime = _dynamicTime.curFrameTime;
	_dynamicTime.curFrameTime = time;
	_dynamicTime.deltaTime = _dynamicTime.curFrameTime - _dynamicTime.lastFrameTime;
	CalcSmoothDeltaTime(_dynamicTime);
	_activeTime = _dynamicTime;

	// Sync _zeroTime with timemanager time
	_zeroTime = GetTimeSinceStartup() - _dynamicTime.curFrameTime;
}

double Time::GetTimeSinceStartup() const
{
	double time = SDL_GetTicks();
	return time / 1000;
}

float Time::GetFramesPerSecond() const
{
	return 1.0f / _dynamicTime.deltaTime;
}

void Time::SetPause(bool pause)
{
	_isTimerPause = pause;
}

void Time::Sleep(unsigned millisecond)
{
#ifdef _WIN32
	::Sleep(millisecond);
#else
	timespec time{ static_cast<time_t>(millisecond / 1000), static_cast<long>((millisecond % 1000) * 1000000) };
	nanosleep(&time, nullptr);
#endif
}

void Time::SetMaximumDeltaTime(float maxStep)
{
	_maximumTimestep = max<float>(maxStep, _dynamicTime.deltaTime);
}

void Time::SetTimeScale(float scale)
{
	bool isOutRange = scale <= 100 && scale >= 0.0f;
	if (isOutRange)
		_timeSpeedScale = scale;
	else
		ErrorString("time speed scale is out of range.Range(0~100).");

}

void Time::OneShotTimer(TimerCallback callback, int msTime)
{
	ShotTimer(callback, msTime, 1);
}

void Time::OneShotTimer(std::function<void()> callback, int msTime)
{
	ShotTimer(callback, msTime, 1);
}

void Time::ShotTimer(TimerCallback callback, int msTime, int count)
{
	Thread timerThread(&This::timerCount,this, callback, msTime, count);
	timerThread.detach();
}

void Time::ShotTimer(std::function<void()> callback, int msTime, int count)
{
	Thread timerThread(&This::timerCountClass, this, callback, msTime, count);
	timerThread.detach();
}

void Time::timerCount(TimerCallback callback, int msTime, int count)
{
	if (count <= 0)
		return;
	int tmpCount = count;
	while (tmpCount--)
	{
		std::chrono::milliseconds dura(msTime);
		std::this_thread::sleep_for(dura);
		(*callback)();
	}
}

void Time::timerCountClass(std::function<void()> callback, int msTime, int count)
{
	if (count <= 0)
		return;
	int tmpCount = count;
	while (tmpCount--)
	{
		std::chrono::milliseconds dura(msTime);
		std::this_thread::sleep_for(dura);
		callback();
	}
}

}