#pragma once
#include "MotionSpace.h"


using namespace Auto3D;
class FreeCamera;
class AntiAliasingSpace : public MotionSpace
{
public:
	explicit AntiAliasingSpace(Ambient* ambient);
	~AntiAliasingSpace();
	void Awake()override;
};


