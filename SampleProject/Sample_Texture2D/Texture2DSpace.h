#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class Texture2DSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(Texture2DSpace)
public:
	void Init()override;
	void Destruct()override;
};


