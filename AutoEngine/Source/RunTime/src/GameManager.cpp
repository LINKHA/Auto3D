#include "GameManager.h"
#include "NewDef.h"



namespace Auto3D {

GameManager::GameManager(SharedPtr<Ambient> ambient)
	:Super(ambient)
{

}
GameManager::~GameManager()
{
}
LevelGameManager::LevelGameManager(SharedPtr<Ambient> ambient)
	:Super(ambient)
{}
LevelGameManager::~LevelGameManager(){}

GlobalGameManager::GlobalGameManager(SharedPtr<Ambient> ambient)
	:Super(ambient)
{}
GlobalGameManager::~GlobalGameManager(){}
}

