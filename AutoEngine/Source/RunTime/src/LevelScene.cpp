#include "LevelScene.h"
#include "Node.h"
#include "Component.h"
#include "PhysicsWorld2D.h"
#include "NewDef.h"


namespace Auto3D {

LevelScene::LevelScene(Ambient* ambient, int levelID)
	: Super(ambient)
{
	_levelID = levelID;
}

LevelScene::~LevelScene()
{
	_nodeToAdd.clear();
	_nodeToRemove.clear();
	_nodes.clear();
}

void LevelScene::Awake() 
{
	_sceneNode = new SceneNode(_ambient, _levelID);
}

void LevelScene::Start() 
{

}

void LevelScene::Update() 
{

}

void LevelScene::FixUpdate() 
{

}

void LevelScene::Finish() 
{

}

void LevelScene::Draw() 
{

}

Node* LevelScene::CreateNode(STRING name)
{
	Node* node = new Node(_ambient, _levelID);
	node->SetName(name);
	return node;
}

void LevelScene::AddNode(Node* node)
{
	Assert(node != NULL);
	if (_isInsideRun)
	{
		_nodeToRemove.remove(node);
		_nodeToAdd.push_back(node);
		return;
	}
	_nodeToAdd.remove(node);
	_nodeToRemove.remove(node);
	_nodes.push_back(node);
}

void LevelScene::RemoveNode(Node* node)
{
	Assert(node != NULL);
	_nodeToAdd.remove(node);
	_nodeToRemove.remove(node);
	if (_isInsideRun)
	{
		_nodeToRemove.push_back(node);
	}
	else
	{
		_nodes.remove(node);
	}
}


void LevelScene::ModeRunNode(RunMode runMode)
{
	if (runMode == RunMode::Default)
	{
		ErrorString("GameObejct fail to Run.");
		return;
	}
	_isInsideRun = true;

	for (LIST<Node*>::iterator i = _nodes.begin(); i != _nodes.end(); i++)
	{
		Node* node = *i;
		if (node && node->GetEnable())
		{
			if (runMode == RunMode::Awake)
				for (PAIR_VECTOR<STRING, Component*>::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Awake();
				}
			else if (runMode == RunMode::Start)
			{
				for (PAIR_VECTOR<STRING, Component*>::iterator it = node->GetComponentsArray().begin(); 
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Start();
				}
			}
			else if (runMode == RunMode::Update)
				for (PAIR_VECTOR<STRING, Component*>::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Update();
				}
			else if (runMode == RunMode::FixUpdate)
				for (PAIR_VECTOR<STRING, Component*>::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->FixUpdate();
				}
			else if (runMode == RunMode::Finish)
			{
				for (PAIR_VECTOR<STRING, Component*>::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Finish();
				}
			}
			else
				ErrorString("GameObejct fail to Run.");
		}
	}
	_isInsideRun = false;
	delayAddRemoveNode();
}

void LevelScene::delayAddRemoveNode()
{
	Assert(!_isInsideRun);
	for (LIST<Node*>::iterator i = _nodeToRemove.begin(); i != _nodeToRemove.end(); /**/)
	{
		Node* node = *i;
		++i;
		RemoveNode(node);
	}
	_nodeToRemove.clear();
	for (LIST<Node*>::iterator i = _nodeToAdd.begin(); i != _nodeToAdd.end(); /**/)
	{
		Node* node = *i;
		++i;
		AddNode(node);
	}
	_nodeToAdd.clear();
}

SceneNode* LevelScene::GetSceneNode()
{
	if (!_sceneNode)
	{
		_sceneNode = new SceneObject(_ambient, _levelID);
		AddNode(_sceneNode);
	}
	return _sceneNode;
}

}