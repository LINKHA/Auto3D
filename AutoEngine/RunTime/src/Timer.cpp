#include "Timer.h"
AUTO_BEGIN
template<> Timer* Singleton<Timer>::m_instance = nullptr;

Timer::Timer()
{
}


Timer::~Timer()
{
}
AUTO_END