#pragma once
#include "GameObject.h"
#include "OpenGLGather.h"
namespace Auto3D {

class RenderComponent : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RenderComponent, Component);
	DECLARE_OBJECT_SERIALIZE(RenderComponent);
public:
	RenderComponent();
	RenderComponent(Ambient*  ambient);
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}

};

}
