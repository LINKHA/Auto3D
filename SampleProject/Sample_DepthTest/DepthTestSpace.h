#pragma once
#include "MotionSpace.h"
USING_AUTO
class DepthTestSpace : public MotionSpace
{
public:
	explicit DepthTestSpace(Ambient* ambient);
	~DepthTestSpace();
	void Awake()override;
	void Start()override;
	void Update()override;
	int Launch();
};

