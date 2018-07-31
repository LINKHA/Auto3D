#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class MoreCameraSpace : public MotionSpace
{
public:
	explicit MoreCameraSpace(Ambient* ambient);
	~MoreCameraSpace();
	void Start()override;
	void Update()override;
	int Launch();
};