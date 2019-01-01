#pragma once

#include "Application.h"

using namespace Auto3D;
class TextureSpace : public Application
{
	REGISTER_SPACE_CLASS(TextureSpace)
public:
	void Init()override;
	void Start()override;
	void Stop()override;
};
