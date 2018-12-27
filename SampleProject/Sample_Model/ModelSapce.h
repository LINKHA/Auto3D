#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class ModelSapce : public MotionSpace
{
	REGISTER_SPACE_CLASS(ModelSapce)
public:
	void Init()override;
	void Destruct()override;
};


