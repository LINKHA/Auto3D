#include "GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"
namespace Auto3D {

SINGLETON_INSTANCE(GameObjectManager);

GameObjectManager::GameObjectManager()
{
}
GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::AddGameObject(GameObject* obj)
{
	Assert(obj != NULL);
	if (_isInsideRun)
	{
		_gameObjectsToRemove.remove(obj);
		_gameObjectsToAdd.push_back(obj);
		return;
	}
	_gameObjectsToAdd.remove(obj);
	_gameObjectsToRemove.remove(obj);
	_gameObjects.push_back(obj);
}

void GameObjectManager::RemoveGameObject(GameObject* obj)
{
	Assert(obj != NULL);
	_gameObjectsToAdd.remove(obj);
	_gameObjectsToRemove.remove(obj);
	if (_isInsideRun)
	{
		_gameObjectsToRemove.push_back(obj);
	}
	else
	{
		_gameObjects.remove(obj);
	}
}
void GameObjectManager::ModeRunGameObject(GameObjectRunMode runMode)
{
	if (runMode == GameObjectRunMode::kDefault)
	{
		ErrorString("GameObejct fail to Run.");
		return;
	}
	_isInsideRun = true;

	for (GameObjectContainer::iterator i = _gameObjects.begin(); i != _gameObjects.end(); i++)
	{
		GameObject* obj = *i;
		if (obj && obj->GetEnable())
		{
			using compomentIt = AUTO_VECTOR(int, Component*)::iterator;
			if (runMode == GameObjectRunMode::kAwake)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++) 
				{ 
					if (k->second->GetEnable())
						k->second->Awake();
				}
			else if (runMode == GameObjectRunMode::kStart)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++)
				{ 
					if (k->second->GetEnable()) 
						k->second->Start(); 
				}
			else if (runMode == GameObjectRunMode::kUpdate)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++) 
				{ 
					if (k->second->GetEnable())
						k->second->Update(); 
				}
			else if (runMode == GameObjectRunMode::kFixUpdate)
				for (compomentIt k = obj->GetComponentsArray().begin(); k != obj->GetComponentsArray().end(); k++) 
				{ 
					if (k->second->GetEnable()) 
						k->second->FixUpdate(); 
				}
			else if (runMode == GameObjectRunMode::kFinish)
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
	delayAddRemoveGameObject();
}


void GameObjectManager::delayAddRemoveGameObject()
{
	Assert(!_isInsideRun);
	for (GameObjectContainer::iterator i = _gameObjectsToRemove.begin(); i != _gameObjectsToRemove.end(); /**/)
	{
		GameObject* obj = *i;
		++i; 
		RemoveGameObject(obj);
	}
	_gameObjectsToRemove.clear();
	for (GameObjectContainer::iterator i = _gameObjectsToAdd.begin(); i != _gameObjectsToAdd.end(); /**/)
	{
		GameObject* obj = *i;
		++i; 
		AddGameObject(obj);
	}
	_gameObjectsToAdd.clear();
}


}