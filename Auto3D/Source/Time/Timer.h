#pragma once
#include "AutoConfig.h"
#include <functional>

namespace Auto3D {

namespace TimerState
{
	enum Type
	{
		DEFAULT,
		INIT,
		RUNNING,
		STOPPING,
		PAUSEING,
	};
};


/// FTimer independent system
class AUTO_API FTimer
{
	typedef void(__cdecl* TimerCallback) ();
public:
	/// The constructor
	FTimer(TimerCallback callback, int interval);
	/// The constructor with class member function
	FTimer(std::function<void()> callback, int interval);
	/// The constructor
	FTimer(TimerCallback callback, int interval, int delayTime);
	/// The constructor with class member function
	FTimer(std::function<void()> callback, int interval, int delayTime);
	/// There is no msTime running once after delayTime (if count is 0, there is no limit)
	FTimer(TimerCallback callback, int interval, int delayTime, int count);
	/// There is no msTime running once after delayTime with class member funcation (if count is 0, there is no limit)
	FTimer(std::function<void()> callback, int interval, int delayTime, int count);
	/// The destructor
	~FTimer() = default;
	/// Stop timer begin from start
	void Stop();
	/// Begin timer
	void Begin();
	/// Pause timer begin from current
	void Pause();
	/// Destory timer but not destructor class
	void Destory();
private:
	/// FTimer count with function
	void TimerCount(TimerCallback callback, int interval, int delayTime, int count);
	/// FTimer count with class member function
	void TimerCountClass(std::function<void()> callback, int interval, int delayTime, int count);
	/// The time interval milliseconds
	int _interval;
	/// Delay time milliseconds
	int _delayTime;
	/// The number of runs is infinite if it's zero
	int _count;
	/// break timer thread flag
	bool _destory{};
	/// stop timer thread flag
	bool _stop{};
	/// pause timer thread flag
	bool _pause{};
	/// timer state
	TimerState::Type _state;
};

}