#pragma once
#include "MotionSpace.h"

using namespace Auto3D;
class FreeCamera;

class TimeSpace : public MotionSpace
{
public:
	explicit TimeSpace(Ambient* ambient);
	~TimeSpace();
	void Awake()override;
};


