#pragma once
#include "MotionSpace.h"

using namespace Auto3D;

class BloomSpace : public MotionSpace
{
public:
	explicit BloomSpace(Ambient* ambient);
	~BloomSpace();
	void Awake()override;
};
