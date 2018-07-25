#pragma once

#include "MotionSpace.h"
#include "Launch.h"
USING_AUTO

class SkyBoxSpace : public MotionSpace
{
public:
	explicit SkyBoxSpace(Ambient* ambient);
	~SkyBoxSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
