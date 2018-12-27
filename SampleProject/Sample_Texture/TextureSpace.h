#pragma once

#include "MotionSpace.h"

using namespace Auto3D;
class TextureSpace : public MotionSpace
{
	REGISTER_SPACE_CLASS(TextureSpace)
public:
	void Init()override;
	void Destruct()override;
};
