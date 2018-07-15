#pragma once
#include "MotionSpace.h"

USING_AUTO

class BloomSpace : public MotionSpace
{
public:
	BloomSpace();
	~BloomSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
