#pragma once

#include "MotionSpace.h"
USING_AUTO

class TextureSpace : public MotionSpace
{
public:
	explicit TextureSpace(AUTO::Ambient* ambient);
	~TextureSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

