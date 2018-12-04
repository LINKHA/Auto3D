#pragma once
#include "MotionSpace.h"
using namespace Auto3D;

class VertexExplodeSpace : public MotionSpace
{
public:
	explicit VertexExplodeSpace(Ambient* ambient);
	~VertexExplodeSpace();
	void Awake()override;
};
