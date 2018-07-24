#pragma once

#include "MotionSpace.h"
USING_AUTO

class VertexExplodeSpace : public MotionSpace
{
public:
	explicit VertexExplodeSpace(Ambient* ambient);
	~VertexExplodeSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
