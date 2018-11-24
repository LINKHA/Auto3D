#pragma once
#include "RenderComponent.h"
namespace Auto3D {

class RenderComponent2D : public RenderComponent
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RenderComponent2D, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(RenderComponent2D);
public:
	explicit RenderComponent2D(Ambient* ambient);
};

}