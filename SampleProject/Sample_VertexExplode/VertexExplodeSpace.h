#pragma once
#include "Application.h"
using namespace Auto3D;

class VertexExplodeSpace : public Application
{
	REGISTER_SPACE_CLASS(VertexExplodeSpace)
public:
	void Init()override;
	void Start()override;
	void Update()override;
	void Stop()override;
};
