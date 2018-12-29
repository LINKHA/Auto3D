#pragma once
#include "LevelBehaviorObject.h"
#include "Node.h"
#include "SceneNode.h"

namespace Auto3D {

const static char* SCENE_ATTACH = "Scene_attach";
const static char* GEOMETRY_ATTACH = "GeoMetry_attach";

class SceneNode;

class Component : public LevelBehaviorObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Component, LevelBehaviorObject)
public:
	explicit Component(Ambient* ambient);
	/**
	* @brief : Get game object quote
	*/
	Node& GetNode();
	/**
	* @brief : Get game object quote const
	*/
	const Node& GetNode() const;
	/**
	* @brief : Get game object ptr
	*/
	SharedPtr<Node> GetNodePtr();
	/**
	* @brief : Get game object ptr const
	*/
	SharedPtr<Node> GetNodePtr() const;
	/**
	* @brief : Mount component for gameobject
	*/
	void SetNode(SharedPtr<Node> node);
	/**
	* @brief : Return vector3 with gameObject position
	*/
	Vector3 GetPosition();
	/**
	* @brief : Return int with scene id
	*/
	const int GetLevelID();
	/**
	* @brief : Return current scene node
	*/
	SharedPtr<SceneNode> GetCurrentSceneNode();
	/**
	* @brief : This component is automatically invoked when registering to a node
	*/
	virtual void Init()
	{
		_levelID = GetNodePtr()->GetLevelID();
	}

private:
	SharedPtr<Node> _node;
};

}