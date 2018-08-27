#include "Scene.h"
#include "GameObject.h"

namespace Auto3D {

Scene::Scene(Ambient* ambient)
	:Super(ambient)
{
}
Scene::~Scene()
{}

void Scene::AddNode(Node* node)
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

void Scene::RemoveNode(Node* node)
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

void Scene::ModeRunNode(NodeRunMode runMode)
{
	if (runMode == NodeRunMode::kDefault)
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
			using compomentIt = AUTO_VECTOR(int, Component*)::iterator;
			if (runMode == NodeRunMode::kAwake)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Awake();
				}
			else if (runMode == NodeRunMode::kStart)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Start();
				}
			else if (runMode == NodeRunMode::kUpdate)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->Update();
				}
			else if (runMode == NodeRunMode::kFixUpdate)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{
					if (k->second->GetEnable())
						k->second->FixUpdate();
				}
			else if (runMode == NodeRunMode::kFinish)
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

void Scene::delayAddRemoveNode()
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