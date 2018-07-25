#pragma once
#include "MotionSpace.h"
	
USING_AUTO
class StencilTestSpace : public MotionSpace
{
public:
	explicit StencilTestSpace(Ambient* ambient);
	~StencilTestSpace();
	void Start()override;
	void Update()override;
	int Launch();
};	
