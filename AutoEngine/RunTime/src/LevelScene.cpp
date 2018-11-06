#include "LevelScene.h"
#include "GameObject.h"
#include "Component.h"

namespace Auto3D {

LevelScene::LevelScene(Ambient* ambient, int levelNumber)
	: Super(ambient)
	, _isEnable(true)
	, _levelNumber(levelNumber)
{
}


LevelScene::~LevelScene()
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

void LevelScene::ModeRunNode(RunMode runMode)
{
	if (runMode == RunMode::kDefault)
	{
		ErrorString("GameObejct fail to Run.");
		return;
	}
	_isInsideRun = true;

	for (NodeContainer::iterator i = _nodes.begin(); i != _nodes.end(); i++)
	{
		GameObject* obj = static_cast<GameObject*>(*i);
		if (obj && obj->GetEnable())
		{
			using compomentIt = PAIR_VECTOR(int, Component*)::iterator;
			if (runMode == RunMode::kAwake)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Awake();
				}
			else if (runMode == RunMode::kStart)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Start();
				}
			else if (runMode == RunMode::kUpdate)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Update();
				}
			else if (runMode == RunMode::kFixUpdate)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->FixUpdate();
				}
			else if (runMode == RunMode::kFinish)
			{
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
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
	for (NodeContainer::iterator i = _nodeToRemove.begin(); i != _nodeToRemove.end(); /**/)
	{
		Node* node = *i;
		++i;
		RemoveNode(node);
	}
	_nodeToRemove.clear();
	for (NodeContainer::iterator i = _nodeToAdd.begin(); i != _nodeToAdd.end(); /**/)
	{
		Node* node = *i;
		++i;
		AddNode(node);
	}
	_nodeToAdd.clear();
}





}