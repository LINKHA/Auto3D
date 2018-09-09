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
public:
	REGISTER_DERIVED_ABSTRACT_CLASS(GameManager, Object);
	DECLARE_OBJECT_SERIALIZE(GameManager);
	explicit GameManager(Ambient* ambient);
};

class LevelGameManager : public GameManager
{
public:
	REGISTER_DERIVED_ABSTRACT_CLASS(LevelGameManager, GameManager);
	DECLARE_OBJECT_SERIALIZE(LevelGameManager);
	explicit LevelGameManager(Ambient* ambient);
};

class GlobalGameManager : public GameManager
{
public:
	REGISTER_DERIVED_ABSTRACT_CLASS(GlobalGameManager, GameManager);
	DECLARE_OBJECT_SERIALIZE(GlobalGameManager);
	explicit GlobalGameManager(Ambient* ambient);
};

}

