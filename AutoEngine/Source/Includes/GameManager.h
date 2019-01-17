#pragma once
#include "Object.h"

#include "Ambient.h"
namespace Auto3D {


/**
* Any game manager (eg. AudioManager, dynamicsmanager) that needs serialization
*/
class GameManager : public Object
{
	REGISTER_OBJECT_ABSTRACT_CLASS(GameManager, Object)
public:
	explicit GameManager(SharedPtr<Ambient> ambient);
};

/** 
* LevelGameManagers are destroyed and reloaded from the new scene when loading a new scene
*/
class LevelGameManager : public GameManager
{
	REGISTER_OBJECT_ABSTRACT_CLASS(LevelGameManager, GameManager)
public:
	explicit LevelGameManager(SharedPtr<Ambient> ambient);
};

/**
* GlobalGameManagers are run all game period
*/
class GlobalGameManager : public GameManager
{
	REGISTER_OBJECT_ABSTRACT_CLASS(GlobalGameManager, GameManager)
public:
	explicit GlobalGameManager(SharedPtr<Ambient> ambient);
};

}

