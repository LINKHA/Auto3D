#pragma once
#include "Object.h"
#include "RunDefines.h"
#include "BehaviorObject.h"
#include "SceneNode.h"

namespace Auto3D {
class Node;

class LevelScene : public BehaviorObject
{
	REGISTER_DERIVED_ABSTRACT_CLASS(LevelScene, BehaviorObject);
	DECLARE_OBJECT_SERIALIZE(LevelScene);
	using NodeContainer = LIST(Node*);
public:
	/**
	* @brief : Register scenario by serial number , 
		default levelNumber 0
	*/
	explicit LevelScene(Ambient* ambient,int id = 0);

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixUpdate();
	virtual void Finish();
	virtual void Draw();
	/**
	* @brief : Add node to _nodeToAdd delay run over to add _nodes
	*/
	void AddNode(Node* node);
	/**
	* @brief : Add node to _nodeToRemove delay run over to remove _nodes
	*/
	void RemoveNode(Node* node);

	Node* CreateNode(STRING name = "")
	{
		Node* node = new Node(_ambient, _sceneID);
		AddNode(node);
		return node;
	}
	void RemoveGameObject(STRING name);

	/**
	* @brief : Process components in all nodes that are opened
	*/
	void ModeRunNode(RunMode runMode);
	/**
	* @brief : Return is load
	*/
	bool IsEnable() { return _isEnable; }
	/**
	* @brief : Set enable
	*/
	void Enable(bool enable) { _isEnable = enable; }
	/**
	* @brief : Return current level number
	*/
	int GetSceneID() { return _sceneID; }
private:
	/**
	* @brief : if not run this function will run once in one frame
	*/
	void delayAddRemoveNode();
protected:
	/// scene id
	int _sceneID{};
private:
	/// scene node (This node has one and only one for each scenario)
	SceneNode* _sceneNode;
	/// all node in this container
	NodeContainer _nodes;
	/// temp memory will add node in frame finish will clear
	NodeContainer _nodeToAdd;
	/// temp memory will remove node in frame finish will clear
	NodeContainer _nodeToRemove;
	/// scene root node
	//SceneObject* _scene;

	/// run flag
	bool _isInsideRun{};
	/// is enable
	bool _isEnable{};
};

}
