#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class DebugSapce : public MotionSpace
{
	REGISTER_SPACE_CLASS(DebugSapce)
public:
	void Init()override;
	void Destruct()override;
};


