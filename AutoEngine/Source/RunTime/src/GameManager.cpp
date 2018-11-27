#include "GameManager.h"
#include "DebugNew.h"



namespace Auto3D {

GameManager::GameManager(Ambient* ambient)
	:Super(ambient)
{

}
GameManager::~GameManager()
{
}
LevelGameManager::LevelGameManager(Ambient* ambient)
	:Super(ambient)
{}
LevelGameManager::~LevelGameManager(){}

GlobalGameManager::GlobalGameManager(Ambient* ambient)
	:Super(ambient)
{}
GlobalGameManager::~GlobalGameManager(){}
}

