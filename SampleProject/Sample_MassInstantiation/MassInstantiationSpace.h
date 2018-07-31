#pragma once

#include "MotionSpace.h"
using namespace Auto3D;

class MassInstantiationSpace : public MotionSpace
{
public:
	explicit MassInstantiationSpace(Ambient* ambient);
	~MassInstantiationSpace();
	void Awake()override;
	void Start()override;
	void Update()override;
	int Launch();
};

