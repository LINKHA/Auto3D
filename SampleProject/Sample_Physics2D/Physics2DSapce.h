#pragma  once
#include "MotionSpace.h"

using namespace Auto3D;
class Physics2DSapce : public MotionSpace
{
	REGISTER_SPACE_CLASS(Physics2DSapce)
public:
	void Init()override;
	void Destruct()override;
	
};


