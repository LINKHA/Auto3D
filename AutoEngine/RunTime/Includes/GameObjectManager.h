#ifndef GAME_OBJECT_MANAGER_H_
#define GAME_OBJECT_MANAGER_H_

#include "GameManager.h"
#include "GameObject.h"
#include "Singleton.h"

AUTO_BEGIN
class Component;
enum GameObjectRunMode
{
	DefaultMode	= -1,
	AwakeMode,
	StartMode,
	UpdateMode,
	FixUpdateMode,
	FinishMode,
};
class GameObjectManager : public LevelGameManager,public Singleton<GameObjectManager>
{
	REGISTER_DERIVED_ABSTRACT_CLASS(GameObjectManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(GameObjectManager);
	typedef _LIST(GameObject*)	GameObjectContainer;

public:
	GameObjectManager();
	void AddGameObject(GameObject * obj);
	void RemoveGameObject(GameObject * obj);

	void ModeRunGameObject(GameObjectRunMode runMode);
	
private:
	void DelayAddRemoveGameObject();
private:
	GameObjectContainer	m_GameObjects;
	GameObjectContainer	m_GameObjectsToAdd;
	GameObjectContainer	m_GameObjectsToRemove;
	//Update or Start now
	bool				m_InsideRun;
};
AUTO_END
#endif // !GAME_OBJECT_MANAGER_H_