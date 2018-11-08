#pragma once
#include "Object.h"
#include "AutoSDL2.h"
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


class Timer
{
	typedef void(__cdecl* TimerCallback) ();
public:
	Timer(TimerCallback callback,int interval)
	{
		Timer(callback,interval, 0, 0);
		
	}
	Timer(TimerCallback callback, int interval,int delayTime)
	{
		Timer(callback, interval, delayTime, 0);
	}
	/**
	* @brief : There is no msTime running once after delayTime 
	*			(if count is 0, there is no limit)
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

	~Timer()
	{
	
	}

	void Stop()
	{
		_stopFlag = true;
		state = TimerState::STOPPING;
	}
	void Begin()
	{
		_stopFlag = false;
		_pauseFlag = false;
		state = TimerState::RUNNING;
	}
	void Pause()
	{
		_pauseFlag = true;
		state = TimerState::PAUSEING;
	}
	void Destory()
	{
		_destoryFlag = true;
	}
private:
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
private:
	/// The time interval milliseconds
	int _interval;
	/// Delay time milliseconds
	int _delayTime;
	/// The number of runs is infinite if it's zero
	int _count;
	/// break timer thread flag
	bool _destoryFlag{};
	
	bool _stopFlag{};

	bool _pauseFlag{};

	TimerState state;
};

}