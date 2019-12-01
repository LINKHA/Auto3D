#include "Time.h"
#include "Debug/Log.h"

#ifdef _WIN32
#	include <Windows.h>
#	include <MMSystem.h>
#else
#	include <sys/time.h>
#	include <unistd.h>
#endif

#include <SDL.h>
#include <thread>
#include <ctime>

#include "Debug/DebugNew.h"

namespace Auto3D
{

SYSTEMTIME  sysTime;

const float MAXIMUM_DELTA_TIME = 1.0f / 3.0f;
const float MINIMUM_DELTA_TIME = 0.00001f;
const float START_UP_DELTA_TIME = 0.02f;
const float NEW_DELTA_TIME_WEIGHT = 0.2f; // for smoothing


FTimeModule::FTimeModule()
{
	_timeSpeedScale = 1.0f;
	_maximumTimestep = MAXIMUM_DELTA_TIME;
	ResetTime();
}
FTimeModule::~FTimeModule()
{
}

void FTimeModule::Update()
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
		SetTime(_dynamicTime._curFrameTime + START_UP_DELTA_TIME * _timeSpeedScale);
		// This is not a real delta time so don't include in smoothed time
		_activeTime._smoothingWeight = 0.0f;
		_dynamicTime._smoothingWeight = 0.0f;
		return;
	}

	double time = GetTimeSinceStartup() - _zeroTime;

	// clamp the delta time in case a frame takes too long.
	if (time - _dynamicTime._curFrameTime > _maximumTimestep)
	{
		SetTime(_dynamicTime._curFrameTime + _maximumTimestep * _timeSpeedScale);
		return;
	}
	// clamp the delta time in case a frame goes to fast! (prevent delta time being zero)
	if (time - _dynamicTime._curFrameTime < MINIMUM_DELTA_TIME)
	{
		SetTime(_dynamicTime._curFrameTime + MINIMUM_DELTA_TIME * _timeSpeedScale);
		return;
	}
	// handle time pause
	if (_isTimerPause)
	{
		SetTime(_dynamicTime._curFrameTime);
		// This is not a real delta time so don't include in smoothed time
		_activeTime._smoothingWeight = 0.0f;
		_dynamicTime._smoothingWeight = 0.0f;
		return;
	}
	// handle time scale
	if (!CompareApproximately(_timeSpeedScale, 1.0f))
	{
		float deltaTime = time - _dynamicTime._curFrameTime;
		SetTime(_dynamicTime._curFrameTime + deltaTime * _timeSpeedScale);
		return;
	}

	_dynamicTime._lastFrameTime = _dynamicTime._curFrameTime;
	_dynamicTime._curFrameTime = time;
	_dynamicTime._deltaTime = _dynamicTime._curFrameTime - _dynamicTime._lastFrameTime;
	CalcSmoothDeltaTime(_dynamicTime);

	_activeTime = _dynamicTime;

}

void FTimeModule::ResetTime()
{
	_dynamicTime._curFrameTime = 0.0f;
	_dynamicTime._lastFrameTime = 0.0f;
	_dynamicTime._deltaTime = 0.0f;
	_dynamicTime._smoothDeltaTime = 0.0f;
	_activeTime = _dynamicTime;
	_isFirstFrameAfterReset = true;
	_isFirstFrameAfterPause = true;
	_isTimerPause = false;
	_frameCount = 0;
	_zeroTime = GetTimeSinceStartup();
}

void FTimeModule::CalcSmoothDeltaTime(TimeHolder& time)
{
	// If existing weight is zero, don't take existing value into account
	time._smoothingWeight *= (1.0f - NEW_DELTA_TIME_WEIGHT);
	time._smoothingWeight += NEW_DELTA_TIME_WEIGHT;
	// As confidence in smoothed value increases the divisor goes towards 1
	float normalized = NEW_DELTA_TIME_WEIGHT / time._smoothingWeight;
	time._smoothDeltaTime = Lerp(time._smoothDeltaTime, time._deltaTime, normalized);
}

FTimeModule::RealTime& FTimeModule::GetRealTime()
{
	GetLocalTime(&sysTime);
	_realTime._year = sysTime.wYear;
	_realTime._month = sysTime.wMonth;
	_realTime._day = sysTime.wDay;
	_realTime._hour = sysTime.wHour;
	_realTime._minute = sysTime.wMinute;
	_realTime._second = sysTime.wSecond;
	return _realTime;
}

void FTimeModule::SetTime(double time)
{
	_dynamicTime._lastFrameTime = _dynamicTime._curFrameTime;
	_dynamicTime._curFrameTime = time;
	_dynamicTime._deltaTime = _dynamicTime._curFrameTime - _dynamicTime._lastFrameTime;
	CalcSmoothDeltaTime(_dynamicTime);
	_activeTime = _dynamicTime;

	// Sync _zeroTime with timemanager time
	_zeroTime = GetTimeSinceStartup() - _dynamicTime._curFrameTime;
}

double FTimeModule::GetTimeSinceStartup() const
{
	double time = SDL_GetTicks();
	return time / 1000;
}

float FTimeModule::GetFramesPerSecond() const
{
	if (_dynamicTime._deltaTime == 0)
		return 60.0;
	return 1.0f / _dynamicTime._deltaTime;
}

void FTimeModule::SetPause(bool pause)
{
	_isTimerPause = pause;
}

void FTimeModule::Sleep(unsigned millisecond)
{
#ifdef _WIN32
	::Sleep(millisecond);
#else
	timespec time{ static_cast<time_t>(millisecond / 1000), static_cast<long>((millisecond % 1000) * 1000000) };
	nanosleep(&time, nullptr);
#endif
}

void FTimeModule::SetMaximumDeltaTime(float maxStep)
{
	_maximumTimestep = Max(maxStep, _dynamicTime._deltaTime);
}

void FTimeModule::SetTimeScale(float scale)
{
	bool isOutRange = scale <= 100 && scale >= 0.0f;
	if (isOutRange)
		_timeSpeedScale = scale;
	else
		ErrorString("time speed scale is out of range.Range(0~100).");

}

void FTimeModule::OneShotTimer(TimerCallback callback, int msTime)
{
	ShotTimer(callback, msTime, 1);
}

void FTimeModule::OneShotTimer(std::function<void()> callback, int msTime)
{
	ShotTimer(callback, msTime, 1);
}

void FTimeModule::ShotTimer(TimerCallback callback, int msTime, int count)
{
	std::thread timerThread(&FTimeModule::TimerCount, this, callback, msTime, count);
	timerThread.detach();
}

void FTimeModule::ShotTimer(std::function<void()> callback, int msTime, int count)
{
	std::thread timerThread(&FTimeModule::TimerCountClass, this, callback, msTime, count);
	timerThread.detach();
}

void FTimeModule::TimerCount(TimerCallback callback, int msTime, int count)
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

void FTimeModule::TimerCountClass(std::function<void()> callback, int msTime, int count)
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

bool FHiresTimer::supported = false;
long long FHiresTimer::frequency = 1000;
FHiresTimer::FHiresTimer()
{
	Reset();
}

long long FHiresTimer::ElapsedUSec(bool reset)
{
	long long currentTime;

#ifdef _WIN32
	if (supported)
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		currentTime = counter.QuadPart;
	}
	else
		currentTime = timeGetTime();
#else
	struct timeval time;
	gettimeofday(&time, 0);
	currentTime = time.tv_sec * 1000000LL + time.tv_usec;
#endif

	long long elapsedTime = currentTime - _startTime;

	// Correct for possible weirdness with changing internal frequency
	if (elapsedTime < 0)
		elapsedTime = 0;

	if (reset)
		_startTime = currentTime;

	return (elapsedTime * 1000000LL) / frequency;
}

void FHiresTimer::Reset()
{
#ifdef _WIN32
	if (supported)
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		_startTime = counter.QuadPart;
	}
	else
		_startTime = timeGetTime();
#else
	struct timeval time;
	gettimeofday(&time, 0);
	_startTime = time.tv_sec * 1000000LL + time.tv_usec;
#endif
}

void FHiresTimer::Initialize()
{
#ifdef _WIN32
	LARGE_INTEGER frequency_;
	if (QueryPerformanceFrequency(&frequency_))
	{
		frequency = frequency_.QuadPart;
		supported = true;
	}
#else
	frequency = 1000000;
	supported = true;
#endif
}

FString TimeStamp()
{
	time_t sysTime;
	time(&sysTime);
	const char* dateTime = ctime(&sysTime);
	return FString(dateTime).Replaced("\n", "");
}

unsigned CurrentTime()
{
	return (unsigned)time(NULL);
}

}
