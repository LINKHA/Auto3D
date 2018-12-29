#pragma once
#include "Component.h"
#include "ILevelBehavior.h"

namespace Auto3D {
class ScriptComponent :public Component , public ILevelBehavior
{
	REGISTER_OBJECT_ABSTRACT_CLASS(ScriptComponent, Component)
public:
	explicit ScriptComponent(Ambient* ambient);

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}

	/**
	* @brief : Create node with name
	*/
	SharedPtr<Node> CreateNode(STRING name = "")override;
};

}

