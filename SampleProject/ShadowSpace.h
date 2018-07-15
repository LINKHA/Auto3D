#pragma once
#include "MotionSpace.h"
USING_AUTO
class ShadowSpace : public MotionSpace
{
public:
	ShadowSpace();
	~ShadowSpace();
	void Start()override;
	void Update()override;
	int Launch();
};


