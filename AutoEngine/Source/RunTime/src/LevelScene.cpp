#include "LevelScene.h"
#include "Node.h"
#include "Component.h"
#include "PhysicsWorld2D.h"


namespace Auto3D {

LevelScene::LevelScene(Ambient* ambient, int id)
	: Super(ambient)
	, _isEnable(true)
	, _sceneID(id)
{	
}

LevelScene::~LevelScene()
{
}

void LevelScene::Awake() 
{
	_sceneNode = new SceneObject(_ambient,_sceneID);
	AddNode(_sceneNode);
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

Node* LevelScene::CreateNode(STRING name)
{
	Node* node = new Node(_ambient, _sceneID);
	AddNode(node);
	return node;
}

void LevelScene::ModeRunNode(RunMode runMode)
{
	if (runMode == RunMode::kDefault)
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
			using compomentIt = PAIR_VECTOR<STRING, Component*>::iterator;
			if (runMode == RunMode::kAwake)
				for (compomentIt k = node->GetComponentsArray().begin(); k != node->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Awake();
				}
			else if (runMode == RunMode::kStart)
				for (compomentIt k = node->GetComponentsArray().begin(); k != node->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Start();
				}
			else if (runMode == RunMode::kUpdate)
				for (compomentIt k = node->GetComponentsArray().begin(); k != node->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Update();
				}
			else if (runMode == RunMode::kFixUpdate)
				for (compomentIt k = node->GetComponentsArray().begin(); k != node->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->FixUpdate();
				}
			else if (runMode == RunMode::kFinish)
			{
				for (compomentIt k = node->GetComponentsArray().begin(); k != node->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Finish();
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
		_sceneNode = new SceneObject(_ambient, _sceneID);
		AddNode(_sceneNode);
	}
	return _sceneNode;
}

}