#include "GameManager.h"



namespace Auto3D {


GameManager::GameManager()
{
}
GameManager::GameManager(Ambient* ambient)
	:Super(ambient)
{

}
GameManager::~GameManager()
{
}

LevelGameManager::LevelGameManager()
{ }
LevelGameManager::LevelGameManager(Ambient* ambient)
	:Super(ambient)
{}
LevelGameManager::~LevelGameManager(){}


GlobalGameManager::GlobalGameManager()
{ }
GlobalGameManager::GlobalGameManager(Ambient* ambient)
	:Super(ambient)
{}
GlobalGameManager::~GlobalGameManager(){}
}

