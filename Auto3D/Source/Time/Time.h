#pragma once
#include "Container/Pair.h"
#include "Container/String.h"
#include "Container/StringHash.h"
#include "Container/Pair.h"
#include "Container/Singleton.h"

#include <functional>

namespace Auto3D
{
const int MIN_FRAMES_PER_SECOND = 10;
const int MAX_FRAMES_PER_SECOND = 200;
/// SubSystem class for time
class AUTO_API FTimeModule
{
	REGISTER_SINGLETON(FTimeModule)

	typedef void(__cdecl* TimerCallback) ();
public:
	/// Save world time information
	using RealTime = struct {
		int _year{};
		int _month{};
		int _day{};
		int _hour{};
		int _minute{};
		int _second{};
	};

	/// Save time information
	using TimeHolder = struct
	{
		/// the reality of the game from now on
		double _curFrameTime{};
		/// the scene begins with the actual use of the previous frame
		double _lastFrameTime{};
		/// time used in the previous frame
		float _deltaTime{};
		/// smoothing delta time weight
		float _smoothingWeight{};
		/// average deltaTime
		float _smoothDeltaTime{};
	};

	/// The constructor
	FTimeModule();
	/// The destructor
	~FTimeModule();
	/// Reset time
	void ResetTime();
	/// SubSystem to engine update
	void Update();
	/// Calculate the average delta time  to smoothDelta
	void CalcSmoothDeltaTime(TimeHolder& time);
	/// Sleep for a number of milliseconds.
	void Sleep(unsigned millisecond);
	/// Set current frame time
	void SetTime(double time);
	/// Set pause key enable
	void SetPause(bool pause);
	/// Set maximum delta time
	void SetMaximumDeltaTime(float maxStep);
	/// Set time scale range(0~100)
	void SetTimeScale(float scale);
	/// Get real time now
	RealTime& GetRealTime();
	/// Return the time from the first frame of seconds.(affected by suspension))
	double GetCurTime() const { return _activeTime._curFrameTime; }
	/// Returns the time when the previous frame was run
	float GetDeltaTime() const { return _activeTime._deltaTime; }
	/// Returns smooth delta time
	float GetSmoothDeltaTime() const { return _activeTime._smoothDeltaTime; }
	/// Return number for engine frame
	unsigned GetFrameCount() const { return _frameCount; }
	/// Return the time from the first frame of seconds. (Not affected by suspension)
	double GetTimeSinceStartup() const;
	/// Return current frames per second.
	float GetFramesPerSecond() const;
	/// Run every msTime for a total of count  (no infinite loop, use FTimer if necessary)
	void ShotTimer(TimerCallback callback, int msTime, int count = 1);
	/// Run every msTime for a total of count with class member function (no infinite loop, use FTimer if necessary)
	void ShotTimer(std::function<void()> callBack, int msTime, int count = 1);
private:
	/// Run every msTime for a total of count (no infinite loop, use FTimer if necessary)
	void TimerCount(TimerCallback callback, int msTime, int count);
	/// Run every msTime for a total of count with class member function (no infinite loop, use FTimer if necessary)
	void TimerCountClass(std::function<void()> callback, int msTime, int count);
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

/// High-resolution operating system timer used in profiling.
class AUTO_API FHiresTimer
{
public:
	/// Construct. Get the starting high-resolution clock value.
	FHiresTimer();
	/// Return elapsed microseconds.
	long long ElapsedUSec(bool reset);
	/// Reset the timer.
	void Reset();
	/// Perform one-time initialization to check support and frequency. Is called automatically at program start.
	static void Initialize();
	/// Return if high-resolution timer is supported.
	static bool IsSupported() { return supported; }
	/// Return high-resolution timer frequency if supported.
	static long long Frequency() { return frequency; }
private:
	/// Starting clock value in CPU ticks.
	long long _startTime;
	/// High-resolution timer support flag.
	static bool supported;
	/// High-resolution timer frequency.
	static long long frequency;
};

/// Return a date/time stamp as a string.
AUTO_API FString TimeStamp();
/// Return current time as seconds since epoch.
AUTO_API unsigned CurrentTime();

}
