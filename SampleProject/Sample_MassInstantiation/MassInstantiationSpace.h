#pragma once

#include "MotionSpace.h"
using namespace Auto3D;

class MassInstantiationSpace : public MotionSpace
{
public:
	explicit MassInstantiationSpace(Ambient* ambient);
	~MassInstantiationSpace();
	void Awake()override;
	int Launch();
};

