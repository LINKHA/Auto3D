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
	
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void FixUpdate();
	virtual void Finish();
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
	/// scene node (This node has one and only one for each scenario)
	SharedPtr<Node> _sceneNode;
	/// all node in this container
	VECTOR<SharedPtr<Node> > _nodes;
	/// temp memory will add node in frame finish will clear
	VECTOR<SharedPtr<Node> > _nodeToAdd;
	/// temp memory will remove node in frame finish will clear
	VECTOR<SharedPtr<Node> > _nodeToRemove;
	/// run flag
	bool _isInsideRun{};
};

}
