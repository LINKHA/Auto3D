#include "Timer.h"

#include <thread>

namespace Auto3D
{

FTimer::FTimer(TimerCallback callback, int interval)
{
	FTimer(callback, interval, 0, 0);
}

FTimer::FTimer(std::function<void()> callback, int interval)
{
	FTimer(callback, interval, 0, 0);
}

FTimer::FTimer(TimerCallback callback, int interval, int delayTime)
{
	FTimer(callback, interval, delayTime, 0);
}

FTimer::FTimer(std::function<void()> callback, int interval, int delayTime)
{
	FTimer(callback, interval, delayTime, 0);
}

FTimer::FTimer(TimerCallback callback, int interval, int delayTime, int count)
	: _interval(interval)
	, _delayTime(delayTime)
	, _count(count)
{
	std::thread timerThread(&FTimer::TimerCount, this, callback, interval, delayTime, count);
	timerThread.detach();
	_state = TimerState::RUNNING;
}

FTimer::FTimer(std::function<void()> callback, int interval, int delayTime, int count)
	: _interval(interval)
	, _delayTime(delayTime)
	, _count(count)
{
	std::thread timerThread(&FTimer::TimerCountClass, this, callback, interval, delayTime, count);
	timerThread.detach();
	_state = TimerState::RUNNING;
}

void FTimer::Stop()
{
	_stop = true;
	_state = TimerState::STOPPING;
}

void FTimer::Begin()
{
	_stop = false;
	_pause = false;
	_state = TimerState::RUNNING;
}

void FTimer::Pause()
{
	_pause = true;
	_state = TimerState::PAUSEING;
}

void FTimer::Destory()
{
	_destory = true;
}

void FTimer::TimerCount(TimerCallback callback, int interval, int delayTime, int count)
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

void FTimer::TimerCountClass(std::function<void()> callback, int interval, int delayTime, int count)
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