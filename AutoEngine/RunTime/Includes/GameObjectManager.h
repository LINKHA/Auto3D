#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Singleton.h"
#include "Mode.h"
namespace Auto3D {
class GameObjectManager : public LevelGameManager,public Singleton<GameObjectManager>
{
	REGISTER_DERIVED_ABSTRACT_CLASS(GameObjectManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(GameObjectManager);
	using GameObjectContainer = _LIST(GameObject*);

public:
	GameObjectManager();
	void AddGameObject(GameObject* obj);
	void RemoveGameObject(GameObject* obj);

	void ModeRunGameObject(RunMode runMode);
	
private:
	void delayAddRemoveGameObject();
private:
	GameObjectContainer	_gameObjects;
	GameObjectContainer _gameObjectsToAdd;
	GameObjectContainer	_gameObjectsToRemove;
	//Update or Start now
	bool				_isInsideRun;
};
}
