#pragma once
#include "GameObject.h"
#include "OpenGLGather.h"
AUTO_BEGIN

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

AUTO_END
