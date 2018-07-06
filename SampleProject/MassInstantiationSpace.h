#pragma once

#include "MotionSpace.h"
USING_AUTO

class MassInstantiationSpace : public MotionSpace
{
public:
	MassInstantiationSpace();
	~MassInstantiationSpace();
	void Awake()override;
	void Start()override;
	void Update()override;
	int Launch();
};

