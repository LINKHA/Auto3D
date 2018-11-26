#pragma once
#include "RenderComponent2D.h"

namespace Auto3D {

class StaticSprite2D : public RenderComponent2D
{
	REGISTER_DERIVED_CLASS(StaticSprite2D, RenderComponent2D);
	DECLARE_OBJECT_SERIALIZE(StaticSprite2D);
public:
	explicit StaticSprite2D(Ambient* ambient);

private:
	
};

}