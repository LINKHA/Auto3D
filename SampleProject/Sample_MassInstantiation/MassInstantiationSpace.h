#pragma once
#include "Application.h"
using namespace Auto3D;

class MassInstantiationSpace : public Application
{
	REGISTER_SPACE_CLASS(MassInstantiationSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};

