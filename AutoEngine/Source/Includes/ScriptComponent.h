#pragma once
#include "Component.h"
#include "ILevelBehavior.h"

namespace Auto3D {
class ScriptComponent :public Component , public ILevelBehavior
{
	REGISTER_OBJECT_ABSTRACT_CLASS(ScriptComponent, Component)
public:
	explicit ScriptComponent(SharedPtr<Ambient> ambient);
	/**
	* @brief : Create node with name
	*/
	SharedPtr<Node> CreateNode(STRING name = "")override;
};

}

