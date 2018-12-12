#pragma once
#include "Object.h"
#include "Singleton.h"
#include "Ambient.h"
namespace Auto3D {


/**
* Any game manager (eg. AudioManager, dynamicsmanager) that needs serialization
* LevelGameManagers are destroyed and reloaded from the new scene when loading a new scene.
* GlobalGameManagers are run all game period
*/
class GameManager : public Object
{
	REGISTER_OBJECT_ABSTRACT_CLASS(GameManager, Object)
public:
	explicit GameManager(Ambient* ambient);
};

class LevelGameManager : public GameManager
{
	REGISTER_OBJECT_ABSTRACT_CLASS(LevelGameManager, GameManager)
public:
	explicit LevelGameManager(Ambient* ambient);
};

class GlobalGameManager : public GameManager
{
	REGISTER_OBJECT_ABSTRACT_CLASS(GlobalGameManager, GameManager)
public:
	explicit GlobalGameManager(Ambient* ambient);
};

}

