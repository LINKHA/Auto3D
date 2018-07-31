#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class DeferredShadingSpace : public MotionSpace
{
public:
	explicit DeferredShadingSpace(Ambient* ambient);
	~DeferredShadingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
