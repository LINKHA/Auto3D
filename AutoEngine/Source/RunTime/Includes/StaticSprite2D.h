#pragma once
#include "RenderComponent2D.h"

namespace Auto3D {

class StaticSprite2D : public RenderComponent2D
{
	REGISTER_OBJECT_CLASS(StaticSprite2D, RenderComponent2D)
public:
	explicit StaticSprite2D(Ambient* ambient);

private:
	
};

}