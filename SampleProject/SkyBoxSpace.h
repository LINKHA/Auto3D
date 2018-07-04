#pragma once

#include "MotionSpace.h"
USING_AUTO

class SkyBoxSpace : public MotionSpace
{
public:
	SkyBoxSpace();
	~SkyBoxSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

