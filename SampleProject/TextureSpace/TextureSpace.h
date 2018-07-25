#pragma once

#include "MotionSpace.h"
//USING_AUTO

USING_AUTO
class TextureSpace : public MotionSpace
{
public:
	explicit TextureSpace(Ambient* ambient);
	~TextureSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
