#pragma once
#include "Object.h"
#include "Node.h"
#include "SceneNode.h"

namespace Auto3D {

const static char* SCENE_ATTACH = "Scene_attach";
const static char* GEOMETRY_ATTACH = "GeoMetry_attach";
const static char* COMPONENT_SET_ATTACH = "Component_Set_attach";

class SceneNode;

class Component : public Object
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Component, Object)
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
	Node* GetNodePtr();
	/**
	* @brief : Get game object ptr const
	*/
	Node* GetNodePtr() const;
	/**
	* @brief : Mount component for gameobject
	*/
	void SetNode(Node* node);
	/**
	* @brief : Set enable
	*/
	void Enable(bool enable) { _isEnable = enable; }
	/**
	* @brief : Return enable
	*/
	bool GetEnable() { return _isEnable; }
	/**
	* @brief : Return vector3 with gameObject position
	*/
	Vector3 GetPosition();
	/**
	* @brief : Return int with scene id
	*/
	const int GetSceneID();
	/**
	* @brief : Return current scene node
	*/
	SceneNode* GetCurrentSceneNode();
	/**
	* @brief : This component is automatically invoked when registering to a node
	*/
	virtual void Init(){}

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
private:
	Node* _node;
	bool _isEnable;
};

}