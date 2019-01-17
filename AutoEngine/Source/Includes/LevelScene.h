#pragma once
#include "Object.h"
#include "LevelBehaviorObject.h"
#include "ILevelBehavior.h"

namespace Auto3D {

class Node;

class LevelScene : public LevelBehaviorObject , public ILevelBehavior
{
	REGISTER_OBJECT_ABSTRACT_CLASS(LevelScene, LevelBehaviorObject)
public:
	/**
	* @brief : Register scenario by serial number , 
		default levelNumber 0
	*/
	explicit LevelScene(SharedPtr<Ambient> ambient,int levelID = 0);
	/** 
	* @brief : All level awake
	*/
	virtual void Awake();
	/**
	* @brief : All level start
	*/
	virtual void Start();
	/**
	* @brief : All level update
	*/
	virtual void Update();
	/**
	* @brief : All level finish
	*/
	virtual void Finish();
	/**
	* @brief : All level draw
	*/
	virtual void Draw();
	/**
	* @brief : Create node with name
	*/
	SharedPtr<Node> CreateNode(STRING name = "")override;
	/**
	* @brief : Add node to _nodeToAdd delay run over to add _nodes
	*/
	void AddNode(SharedPtr<Node> node);
	/**
	* @brief : Add node to _nodeToRemove delay run over to remove _nodes
	*/
	void RemoveNode(SharedPtr<Node> node);
	/**
	* @brief : Process components in all nodes that are opened
	*/
	void ModeRunNode(RunMode runMode);
	/**
	* @brief : Get scene node
	*/
	SharedPtr<Node> GetSceneNode();
	/**
	* @brief : Get nodes
	*/
	VECTOR<SharedPtr<Node> >& GetNodes() { return _nodes; }
private:
	/**
	* @brief : if not run this function will run once in one frame
	*/
	void delayAddRemoveNode();
private:
	/// Scene node (This node has one and only one for each scenario)
	SharedPtr<Node> _sceneNode;
	/// All node in this container
	VECTOR<SharedPtr<Node> > _nodes;
	/// Temp memory will add node in frame finish will clear
	VECTOR<SharedPtr<Node> > _nodeToAdd;
	/// Temp memory will remove node in frame finish will clear
	VECTOR<SharedPtr<Node> > _nodeToRemove;
	/// Run flag
	bool _isInsideRun{};
};

}
