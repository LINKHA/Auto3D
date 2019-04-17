#include "Timer.h"

#include <thread>

namespace Auto3D
{

Timer::Timer(TimerCallback callback, int interval)
{
	Timer(callback, interval, 0, 0);
}

Timer::Timer(std::function<void()> callback, int interval)
{
	Timer(callback, interval, 0, 0);
}

Timer::Timer(TimerCallback callback, int interval, int delayTime)
{
	Timer(callback, interval, delayTime, 0);
}

Timer::Timer(std::function<void()> callback, int interval, int delayTime)
{
	Timer(callback, interval, delayTime, 0);
}

Timer::Timer(TimerCallback callback, int interval, int delayTime, int count)
	: _interval(interval)
	, _delayTime(delayTime)
	, _count(count)
{
	std::thread timerThread(&Timer::TimerCount, this, callback, interval, delayTime, count);
	timerThread.detach();
	_state = TimerState::RUNNING;
}

Timer::Timer(std::function<void()> callback, int interval, int delayTime, int count)
	: _interval(interval)
	, _delayTime(delayTime)
	, _count(count)
{
	std::thread timerThread(&Timer::TimerCountClass, this, callback, interval, delayTime, count);
	timerThread.detach();
	_state = TimerState::RUNNING;
}

void Timer::Stop()
{
	_stop = true;
	_state = TimerState::STOPPING;
}

void Timer::Begin()
{
	_stop = false;
	_pause = false;
	_state = TimerState::RUNNING;
}

void Timer::Pause()
{
	_pause = true;
	_state = TimerState::PAUSEING;
}

void Timer::Destory()
{
	_destory = true;
}

void Timer::TimerCount(TimerCallback callback, int interval, int delayTime, int count)
{
	std::chrono::milliseconds dura(delayTime);
	std::this_thread::sleep_for(dura);
	if (count < 0)
		return;
	if (count == 0)
		for(;;)
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

void Timer::TimerCountClass(std::function<void()> callback, int interval, int delayTime, int count)
{
	std::chrono::milliseconds dura(delayTime);
	std::this_thread::sleep_for(dura);
	if (count < 0)
		return;
	if (count == 0)
		for (;;)
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



}