#pragma once
#include "MotionSpace.h"
USING_AUTO

class MoreCameraSpace : public MotionSpace
{
public:
	explicit MoreCameraSpace(Ambient* ambient);
	~MoreCameraSpace();
	void Start()override;
	void Update()override;
	int Launch();
};