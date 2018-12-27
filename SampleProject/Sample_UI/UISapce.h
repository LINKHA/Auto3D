#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class UISapce : public MotionSpace
{
	REGISTER_SPACE_CLASS(UISapce)
public:
	void Init()override;
	void Destruct()override;
};


