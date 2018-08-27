#pragma once
#include "GameManager.h"
#include "stl_use.h"

namespace Auto3D {

enum class NodeRunMode
{
	kDefault = -1,
	kAwake,
	kStart,
	kUpdate,
	kFixUpdate,
	kFinish,
};
class Node;
class Scene : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Scene, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Scene);
	using NodeContainer = _LIST(Node*);
public:
	explicit Scene(Ambient* ambient);
	/**
	* @brief : Add node to _nodeToAdd delay run over to add _nodes
	*/
	void AddNode(Node* node);
	/**
	* @brief : Add node to _nodeToRemove delay run over to remove _nodes
	*/
	void RemoveNode(Node* node);
	/**
	* @brief : Process components in all nodes that are opened
	*/
	void ModeRunNode(NodeRunMode runMode);
private:
	/**
	* @brief : if not run this function will run once in one frame
	*/
	void delayAddRemoveNode();
private:
	///all node in this container
	NodeContainer _nodes;
	///temp memory will add node in frame finish will clear
	NodeContainer _nodeToAdd;
	///temp memory will remove node in frame finish will clear
	NodeContainer _nodeToRemove;
	///run flag
	bool _isInsideRun{};
};

}

