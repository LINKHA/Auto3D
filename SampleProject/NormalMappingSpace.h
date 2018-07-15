#pragma once
#include "MotionSpace.h"
USING_AUTO
class NormalMappingSpace : public MotionSpace
{
public:
	NormalMappingSpace();
	~NormalMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

