#pragma once
#include "MotionSpace.h"
using namespace Auto3D;
class ShadowSpace : public MotionSpace
{
public:
	explicit ShadowSpace(Ambient* ambient);
	~ShadowSpace();
	void Start()override;
	void Update()override;

	void ShadowNormal();
	void ShadowPoint();
	int Launch();
};


