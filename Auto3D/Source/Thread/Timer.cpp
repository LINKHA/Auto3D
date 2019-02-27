#include "Timer.h"

#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#include <MMSystem.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "../Debug/DebugNew.h"

namespace Auto3D
{

/// \cond PRIVATE
class TimerInitializer
{
public:
    TimerInitializer()
    {
        HiresTimer::Initialize();
        #ifdef _WIN32
        timeBeginPeriod(1);
        #endif
    }

    ~TimerInitializer()
    {
        #ifdef _WIN32
        timeEndPeriod(1);
        #endif
    }
};
/// \endcond

static TimerInitializer initializer;

bool HiresTimer::_supported = false;
long long HiresTimer::_frequency = 1000;

Timer::Timer()
{
	RegisterSubsystem(this);
    Reset();
}

unsigned Timer::ElapsedMSec()
{
    #ifdef _WIN32
    unsigned currentTime = timeGetTime();
    #else
    struct timeval _time;
    gettimeofday(&_time, 0);
    unsigned currentTime = _time.tv_sec * 1000 + _time.tv_usec / 1000;
    #endif
    
    return currentTime - _startTime;
}

void Timer::Reset()
{
    #ifdef _WIN32
    _startTime = timeGetTime();
    #else
    struct timeval _time;
    gettimeofday(&_time, 0);
    _startTime = _time.tv_sec * 1000 + _time.tv_usec / 1000;
    #endif
}

HiresTimer::HiresTimer()
{
    Reset();
}

long long HiresTimer::ElapsedUSec()
{
    long long currentTime;
    
    #ifdef _WIN32
    if (_supported)
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        currentTime = counter.QuadPart;
    }
    else
        currentTime = timeGetTime();
    #else
    struct timeval _time;
    gettimeofday(&_time, 0);
    currentTime = _time.tv_sec * 1000000LL + _time.tv_usec;
    #endif
    
    long long elapsedTime = currentTime - _startTime;
    
    // Correct for possible weirdness with changing internal frequency
    if (elapsedTime < 0)
        elapsedTime = 0;
    
    return (elapsedTime * 1000000LL) / _frequency;
}

void HiresTimer::Reset()
{
    #ifdef _WIN32
    if (_supported)
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        _startTime = counter.QuadPart;
    }
    else
        _startTime = timeGetTime();
    #else
    struct timeval _time;
    gettimeofday(&_time, 0);
    _startTime = _time.tv_sec * 1000000LL + _time.tv_usec;
    #endif
}

void HiresTimer::Initialize()
{
    #ifdef _WIN32
    LARGE_INTEGER frequency_;
    if (QueryPerformanceFrequency(&frequency_))
    {
        _frequency = frequency_.QuadPart;
        _supported = true;
    }
    #else
    _frequency = 1000000;
    _supported = true;
    #endif
}

String TimeStamp()
{
    time_t sysTime;
    time(&sysTime);
    const char* dateTime = ctime(&sysTime);
    return String(dateTime).Replaced("\n", "");
}

unsigned CurrentTime()
{
    return (unsigned)time(NULL);
}

}
