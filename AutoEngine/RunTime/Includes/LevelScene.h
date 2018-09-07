#pragma once
#include "Object.h"
#include "RunDefines.h"
#include "StateVessel.h"

namespace Auto3D {
class Node;

class LevelScene : public StateVessel
{
	REGISTER_DERIVED_ABSTRACT_CLASS(LevelScene, StateVessel);
	DECLARE_OBJECT_SERIALIZE(LevelScene);
	using NodeContainer = _LIST(Node*);
public:
	/**
	* @brief : Register scenario by serial number , 
		default levelNumber 0
	*/
	explicit LevelScene(Ambient* ambient,int levelNumber = 0);
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
	int GetLevelNumber() { return _levelNumber; }
private:
	/**
	* @brief : if not run this function will run once in one frame
	*/
	void delayAddRemoveNode();
protected:
	/// level number
	int _levelNumber{};
private:
	///all node in this container
	NodeContainer _nodes;
	///temp memory will add node in frame finish will clear
	NodeContainer _nodeToAdd;
	///temp memory will remove node in frame finish will clear
	NodeContainer _nodeToRemove;
	///run flag
	bool _isInsideRun{};
	///is enable
	bool _isEnable{};
};

}
