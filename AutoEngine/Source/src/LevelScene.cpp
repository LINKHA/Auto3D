#include "LevelScene.h"
#include "Node.h"
#include "Component.h"
#include "Scene.h"
#include "PhysicsWorld.h"
#include "PhysicsWorld2D.h"
#include "Transform.h"

#include "NewDef.h"


namespace Auto3D {

LevelScene::LevelScene(SharedPtr<Ambient> ambient, int levelID)
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
	_sceneNode = CreateNode("SceneNode");
	_sceneNode->CreateComponent<PhysicsWorld2D>();
	_sceneNode->CreateComponent<PhysicsWorld>();
}

void LevelScene::Start() 
{

}

void LevelScene::Update() 
{

}

void LevelScene::Finish() 
{

}

void LevelScene::Draw() 
{

}

SharedPtr<Node> LevelScene::CreateNode(STRING name)
{
	SharedPtr<Node> node = MakeShared<Node>(_ambient, _levelID);
	GetSubSystem<Scene>()->GetLevelScene(_levelID)->AddNode(node);
	node->SetName(name);
	//Each node must have a Transform component and cannot be deleted
	node->CreateComponent<Transform>();
	return node;
}

void LevelScene::AddNode(SharedPtr<Node> node)
{
	Assert(node != NULL);
	if (_isInsideRun)
	{
		VectorFindEarse(_nodeToRemove, node);
		_nodeToAdd.push_back(node);
		return;
	}
	VectorFindEarse(_nodeToAdd, node);
	VectorFindEarse(_nodeToRemove, node);
	_nodes.push_back(node);
}

void LevelScene::RemoveNode(SharedPtr<Node> node)
{
	Assert(node != NULL);
	VectorFindEarse(_nodeToAdd, node);
	VectorFindEarse(_nodeToRemove, node);
	if (_isInsideRun)
	{
		_nodeToRemove.push_back(node);
	}
	else
	{
		VectorFindEarse(_nodes, node);
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

	for (VECTOR<SharedPtr<Node> >::iterator i = _nodes.begin(); i != _nodes.end(); i++)
	{
		SharedPtr<Node> node = *i;
		if (node && node->GetEnable())
		{
			if (runMode == RunMode::Awake)
				for (PAIR_VECTOR<STRING, SharedPtr<Component> >::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Awake();
				}
			else if (runMode == RunMode::Start)
			{
				for (PAIR_VECTOR<STRING, SharedPtr<Component> >::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Start();
				}
			}
			else if (runMode == RunMode::Update)
				for (PAIR_VECTOR<STRING, SharedPtr<Component> >::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->Update();
				}
			else if (runMode == RunMode::FixUpdate)
				for (PAIR_VECTOR<STRING, SharedPtr<Component> >::iterator it = node->GetComponentsArray().begin();
					it != node->GetComponentsArray().end(); it++)
				{
					if (it->second->IsEnable())
						it->second->FixUpdate();
				}
			else if (runMode == RunMode::Finish)
			{
				for (PAIR_VECTOR<STRING, SharedPtr<Component> >::iterator it = node->GetComponentsArray().begin();
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
	for (VECTOR<SharedPtr<Node> >::iterator i = _nodeToRemove.begin(); i != _nodeToRemove.end(); /**/)
	{
		SharedPtr<Node> node = *i;
		++i;
		RemoveNode(node);
	}
	_nodeToRemove.clear();
	for (VECTOR<SharedPtr<Node> >::iterator i = _nodeToAdd.begin(); i != _nodeToAdd.end(); /**/)
	{
		SharedPtr<Node> node = *i;
		++i;
		AddNode(node);
	}
	_nodeToAdd.clear();
}

SharedPtr<Node> LevelScene::GetSceneNode()
{
	if (!_sceneNode)
	{
		_sceneNode = CreateNode("SceneNode");
	}
	return _sceneNode;
}

}