#pragma once

#include "MotionSpace.h"
#include "Launch.h"
using namespace Auto3D;

class SkyBoxSpace : public MotionSpace
{
public:
	explicit SkyBoxSpace(Ambient* ambient);
	~SkyBoxSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
