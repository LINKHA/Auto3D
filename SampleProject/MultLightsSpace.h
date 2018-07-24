#pragma once
#include "MotionSpace.h"
USING_AUTO

class MultLightsSpace : public MotionSpace
{
public:
	explicit MultLightsSpace(Ambient* ambient);
	~MultLightsSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

