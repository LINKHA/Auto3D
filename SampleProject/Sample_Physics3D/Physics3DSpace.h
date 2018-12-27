#pragma  once
#include "MotionSpace.h"

using namespace Auto3D;

class Physics3DSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(Physics3DSpace)
public:
	void Init()override;
	void Destruct()override;
};


