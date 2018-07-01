#pragma once
#include "MotionSpace.h"
	
USING_AUTO
class StencilTestSpace : public MotionSpace
{
public:
	StencilTestSpace();
	~StencilTestSpace();
	void Awake()override;
	void Start()override;
	void Update()override;
	int Launch();
};	
