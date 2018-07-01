#pragma once

#include "MotionSpace.h"
USING_AUTO

class TextureSpace : public MotionSpace
{
public:
	TextureSpace();
	~TextureSpace();
	void Start()override;
	void Update()override;
	int Launch();
};

