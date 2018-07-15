#pragma once
#include "GameObject.h"
#include "OpenGLGather.h"
AUTO_BEGIN

class RanderComponent : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RanderComponent, Component);
	DECLARE_OBJECT_SERIALIZE(RanderComponent);
public:
	RanderComponent();
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw(Camera* camera = nullptr) {}

};

AUTO_END
