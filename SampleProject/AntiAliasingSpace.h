#pragma once
#include "MotionSpace.h"
USING_AUTO
class AntiAliasingSpace : public MotionSpace
{
public:
	AntiAliasingSpace();
	~AntiAliasingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


