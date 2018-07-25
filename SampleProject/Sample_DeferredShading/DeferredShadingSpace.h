#pragma once
#include "MotionSpace.h"
USING_AUTO
class DeferredShadingSpace : public MotionSpace
{
public:
	explicit DeferredShadingSpace(Ambient* ambient);
	~DeferredShadingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
