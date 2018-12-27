#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class AudioSapce : public MotionSpace
{
	REGISTER_SPACE_CLASS(AudioSapce)
public:
	void Init()override;
	void Destruct()override;
};