#pragma once
#include "RenderComponent.h"
namespace Auto3D {

class RenderComponent2D : public RenderComponent
{
	REGISTER_OBJECT_ABSTRACT_CLASS(RenderComponent2D, RenderComponent)
public:
	explicit RenderComponent2D(Ambient* ambient);
};

}