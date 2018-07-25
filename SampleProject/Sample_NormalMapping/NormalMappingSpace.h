#pragma once
#include "MotionSpace.h"
USING_AUTO
class NormalMappingSpace : public MotionSpace
{
public:
	explicit NormalMappingSpace(Ambient* ambient);
	~NormalMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

