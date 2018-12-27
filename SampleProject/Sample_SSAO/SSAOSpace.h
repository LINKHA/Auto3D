#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class SSAOSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(SSAOSpace)
public:
	void Init()override;
	void Destruct()override;
};


