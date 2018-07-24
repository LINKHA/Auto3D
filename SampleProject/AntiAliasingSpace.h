#pragma once
#include "MotionSpace.h"
USING_AUTO
class AntiAliasingSpace : public MotionSpace
{
public:
	explicit AntiAliasingSpace(Ambient* ambient);
	~AntiAliasingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


