#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class AntiAliasingSpace : public MotionSpace
{
public:
	explicit AntiAliasingSpace(Ambient* ambient);
	~AntiAliasingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


