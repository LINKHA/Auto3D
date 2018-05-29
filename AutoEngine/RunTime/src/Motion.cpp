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

AUTO_END