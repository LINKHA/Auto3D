#pragma once
#include "../Object/Object.h"
#include "../Base/String.h"
#include "../AutoConfig.h"


namespace Auto3D
{

/// Low-resolution operating system timer.
class AUTO_API Timer : public Object
{
	REGISTER_OBJECT_CLASS(Timer, Object)
public:
    /// Construct. Get the starting clock value.
    Timer();

	~Timer() = default;
    /// Return elapsed milliseconds.
    unsigned ElapsedMSec();
    /// Reset the timer.
    void Reset();
    
private:
    /// Starting clock value in milliseconds.
    unsigned _startTime;
};

/// High-resolution operating system timer used in profiling.
class AUTO_API HiresTimer
{
public:
    /// Construct. Get the starting high-resolution clock value.
    HiresTimer();
    
    /// Return elapsed microseconds.
    long long ElapsedUSec();
    /// Reset the timer.
    void Reset();

    /// Perform one-time initialization to check support and frequency. Is called automatically at program start.
    static void Initialize();
    /// Return if high-resolution timer is supported.
    static bool IsSupported() { return _supported; }
    /// Return high-resolution timer frequency if supported.
    static long long Frequency() { return _frequency; }

private:
    /// Starting clock value in CPU ticks.
    long long _startTime;

    /// High-resolution timer support flag.
    static bool _supported;
    /// High-resolution timer frequency.
    static long long _frequency;
};

/// Return a date/time stamp as a string.
AUTO_API String TimeStamp();
/// Return current time as seconds since epoch.
AUTO_API unsigned CurrentTime();

}
