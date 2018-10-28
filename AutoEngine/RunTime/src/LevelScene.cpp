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
		_nodeToRemove.Remove(node);
		_nodeToAdd.PushBack(node);
		return;
	}
	_nodeToAdd.Remove(node);
	_nodeToRemove.Remove(node);
	_nodes.PushBack(node);
}

void LevelScene::RemoveNode(Node* node)
{
	Assert(node != NULL);
	_nodeToAdd.Remove(node);
	_nodeToRemove.Remove(node);
	if (_isInsideRun)
	{
		_nodeToRemove.PushBack(node);
	}
	else
	{
		_nodes.Remove(node);
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

	for (NodeContainer::Iterator i = _nodes.Begin(); i != _nodes.End(); i++)
	{
		GameObject* obj = static_cast<GameObject*>(*i);
		if (obj && obj->GetEnable())
		{
			using compomentIt = HASH_MAP(int, Component*)::Iterator;
			if (runMode == RunMode::kAwake)
				for (compomentIt k = obj->GetComponentsArray().Begin(); k != obj->GetComponentsArray().End(); k++)
				{
					if (k->second->GetEnable())
						k->second->Awake();
				}
			else if (runMode == RunMode::kStart)
				for (compomentIt k = obj->GetComponentsArray().Begin(); k != obj->GetComponentsArray().End(); k++)
				{
					if (k->second->GetEnable())
						k->second->Start();
				}
			else if (runMode == RunMode::kUpdate)
				for (compomentIt k = obj->GetComponentsArray().Begin(); k != obj->GetComponentsArray().End(); k++)
				{
					if (k->second->GetEnable())
						k->second->Update();
				}
			else if (runMode == RunMode::kFixUpdate)
				for (compomentIt k = obj->GetComponentsArray().Begin(); k != obj->GetComponentsArray().End(); k++)
				{
					if (k->second->GetEnable())
						k->second->FixUpdate();
				}
			else if (runMode == RunMode::kFinish)
			{
				for (compomentIt k = obj->GetComponentsArray().Begin(); k != obj->GetComponentsArray().End(); k++)
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
	for (NodeContainer::Iterator i = _nodeToRemove.Begin(); i != _nodeToRemove.End(); /**/)
	{
		Node* node = *i;
		++i;
		RemoveNode(node);
	}
	_nodeToRemove.Clear();
	for (NodeContainer::Iterator i = _nodeToAdd.Begin(); i != _nodeToAdd.End(); /**/)
	{
		Node* node = *i;
		++i;
		AddNode(node);
	}
	_nodeToAdd.Clear();
}





}