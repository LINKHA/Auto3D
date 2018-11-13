#pragma once
#include "Node.h"

namespace Auto3D {
//class SceneNode;
using SceneObject = class SceneNode;

class SceneNode : public Node
{
	REGISTER_DERIVED_CLASS(SceneNode, Node);
	DECLARE_OBJECT_SERIALIZE(SceneNode);
public:
	explicit SceneNode(Ambient* ambient,int id);
	/**
	* @brief : Get scene ID
	*/
	const int GetID() const { return _sceneID; }

private:
	int _sceneID{};

};

}