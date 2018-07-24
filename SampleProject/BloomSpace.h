#pragma once
#include "MotionSpace.h"

USING_AUTO

class BloomSpace : public MotionSpace
{
public:
	explicit BloomSpace(Ambient* ambient);
	~BloomSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
