#include "DepthSet.h"
AUTO_BEGIN
SINGLETON_INSTANCE(DepthSet);
DepthSet::DepthSet()
{
	SetMode(DEPTH_LESS);
}
DepthSet::~DepthSet()
{}
void DepthSet::SetMode(DepthMode mode)
{
	Mode = mode;
}

AUTO_END