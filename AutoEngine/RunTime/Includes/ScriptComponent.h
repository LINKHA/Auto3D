#ifndef SCRIPT_COMPONENT_H_
#define SCRIPT_COMPONENT_H_
#include "GameObject.h"

AUTO_BEGIN
class ScriptComponent :public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ScriptComponent, Component);
	DECLARE_OBJECT_SERIALIZE(ScriptComponent);
public:
	ScriptComponent();
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update(Camera * cam = nullptr) {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
};

AUTO_END

#endif // SCRIPT_COMPONENT_H_
