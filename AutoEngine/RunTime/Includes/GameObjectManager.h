#pragma once
#include "GameManager.h"
#include "GameObject.h"
#include "Singleton.h"
namespace Auto3D {

enum class GameObjectRunMode
{
	kDefault = -1,
	kAwake,
	kStart,
	kUpdate,
	kFixUpdate,
	kFinish,
};


class GameObjectManager : public LevelGameManager,public Singleton<GameObjectManager>
{
	REGISTER_DERIVED_ABSTRACT_CLASS(GameObjectManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(GameObjectManager);
	using GameObjectContainer = _LIST(GameObject*);

public:
	GameObjectManager();
	void AddGameObject(GameObject* obj);
	void RemoveGameObject(GameObject* obj);

	void ModeRunGameObject(GameObjectRunMode runMode);
	
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
