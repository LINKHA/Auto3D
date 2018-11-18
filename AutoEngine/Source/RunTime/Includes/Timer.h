#pragma once
#include "Object.h"
#include "AutoSDL2.h"
#include <functional>
#include <thread>

namespace Auto3D {

enum class TimerState
{
	DEFAULT,
	INIT,
	RUNNING,
	STOPPING,
	PAUSEING
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
		std::thread timerThread(&Timer::timerCount, this, callback, interval, delayTime, count);
		timerThread.detach();
		state = TimerState::RUNNING;
	}
	/**
	* @brief : There is no msTime running once after delayTime with class member funcation (if count is 0, there is no limit)
	*/
	Timer(std::function<void()> callback, int interval, int delayTime, int count)
		: _interval(interval)
		, _delayTime(delayTime)
		, _count(count)
	{
		std::thread timerThread(&Timer::timerCountClass, this, callback, interval, delayTime, count);
		timerThread.detach();
		state = TimerState::RUNNING;
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
		state = TimerState::STOPPING;
	}
	/**
	* @brief : Begin timer 
	*/
	void Begin()
	{
		_stopFlag = false;
		_pauseFlag = false;
		state = TimerState::RUNNING;
	}
	/**
	* @brief : Pause timer begin from current
	*/
	void Pause()
	{
		_pauseFlag = true;
		state = TimerState::PAUSEING;
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