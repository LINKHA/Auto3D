#pragma once
#include "Object.h"
#include "AutoSDL.h"
#include <functional>
#include "AutoThread.h"

namespace Auto3D {

enum class TimerState
{
	Default,
	Init,
	Running,
	Stopping,
	Pauseing,
};

/**
* Timer independent system
*/
class Timer
{
	typedef void(__cdecl* TimerCallback) ();
public:
	/**
	* @brief : The constructor
	*/
	Timer(TimerCallback callback,int interval)
	{
		Timer(callback,interval, 0, 0);
		
	}
	/**
	* @brief : The constructor with class member function
	*/
	Timer(std::function<void()> callback, int interval)
	{
		Timer(callback, interval, 0, 0);
	}
	/**
	* @brief : The constructor 
	*/
	Timer(TimerCallback callback, int interval,int delayTime)
	{
		Timer(callback, interval, delayTime, 0);
	}
	/**
	* @brief :  The constructor with class member function
	*/
	Timer(std::function<void()> callback, int interval, int delayTime)
	{
		Timer(callback, interval, delayTime, 0);
	}
	/**
	* @brief : There is no msTime running once after delayTime (if count is 0, there is no limit)
	*/
	Timer(TimerCallback callback,int interval,int delayTime,int count)
		: _interval(interval)
		, _delayTime(delayTime)
		, _count(count)
	{
		Thread timerThread(&Timer::timerCount, this, callback, interval, delayTime, count);
		timerThread.detach();
		state = TimerState::Running;
	}
	/**
	* @brief : There is no msTime running once after delayTime with class member funcation (if count is 0, there is no limit)
	*/
	Timer(std::function<void()> callback, int interval, int delayTime, int count)
		: _interval(interval)
		, _delayTime(delayTime)
		, _count(count)
	{
		Thread timerThread(&Timer::timerCountClass, this, callback, interval, delayTime, count);
		timerThread.detach();
		state = TimerState::Running;
	}
	/**
	* @brief : The destructor
	*/
	~Timer() = default;
	/**
	* @brief : Stop timer begin from start
	*/
	void Stop()
	{
		_stopFlag = true;
		state = TimerState::Stopping;
	}
	/**
	* @brief : Begin timer 
	*/
	void Begin()
	{
		_stopFlag = false;
		_pauseFlag = false;
		state = TimerState::Running;
	}
	/**
	* @brief : Pause timer begin from current
	*/
	void Pause()
	{
		_pauseFlag = true;
		state = TimerState::Pauseing;
	}
	/**
	* @brief : Destory timer but not destructor class
	*/
	void Destory()
	{
		_destoryFlag = true;
	}
private:
	/**
	* @brief : Timer count with function
	*/
	void timerCount(TimerCallback callback, int interval, int delayTime,int count)
	{
		std::chrono::milliseconds dura(delayTime);
		std::this_thread::sleep_for(dura);
		if (count < 0)
			return;
		if(count==0)
			while (1)
			{
				std::chrono::milliseconds dura(interval);
				std::this_thread::sleep_for(dura);

				(*callback)();
			}
		int tmpCount = count;
		while (tmpCount--)
		{
			std::chrono::milliseconds dura(interval);
			std::this_thread::sleep_for(dura);
			(*callback)();
		}
	}
	/**
	* @brief : Timer count with class member function
	*/
	void timerCountClass(std::function<void()> callback, int interval, int delayTime, int count)
	{
		std::chrono::milliseconds dura(delayTime);
		std::this_thread::sleep_for(dura);
		if (count < 0)
			return;
		if (count == 0)
			while (1)
			{
				std::chrono::milliseconds dura(interval);
				std::this_thread::sleep_for(dura);

				callback();
			}
		int tmpCount = count;
		while (tmpCount--)
		{
			std::chrono::milliseconds dura(interval);
			std::this_thread::sleep_for(dura);
			callback();
		}
	}
private:
	/// The time interval milliseconds
	int _interval;
	/// Delay time milliseconds
	int _delayTime;
	/// The number of runs is infinite if it's zero
	int _count;
	/// break timer thread flag
	bool _destoryFlag{};
	/// stop timer thread flag
	bool _stopFlag{};
	/// pause timer thread flag
	bool _pauseFlag{};
	/// timer state
	TimerState state;
};

}