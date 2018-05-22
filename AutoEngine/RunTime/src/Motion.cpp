#include "Motion.h"

AUTO_BEGIN
template<> MotionManager* Singleton<MotionManager>::m_instance = nullptr;

MotionManager::MotionManager()
{}
MotionManager::~MotionManager()
{}

Motion::Motion()
{
}

Motion::~Motion()
{
}

void Motion::Enable(bool enable)
{
	m_enable = enable;
}
AUTO_END