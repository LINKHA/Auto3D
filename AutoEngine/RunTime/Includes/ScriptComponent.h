#pragma once
#include "GameObject.h"

AUTO_BEGIN
class ScriptComponent :public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ScriptComponent, Component);
	DECLARE_OBJECT_SERIALIZE(ScriptComponent);
public:
	ScriptComponent(Ambient* ambient);
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
};

AUTO_END

