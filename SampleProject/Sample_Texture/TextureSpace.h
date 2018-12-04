#pragma once

#include "MotionSpace.h"

using namespace Auto3D;
class TextureSpace : public MotionSpace
{
public:
	explicit TextureSpace(Ambient* ambient);
	~TextureSpace();
	void Awake()override;
};
