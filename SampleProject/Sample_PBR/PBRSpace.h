#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class PBRSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(PBRSpace)
public:
	void Init()override;
	void Destruct()override;
};