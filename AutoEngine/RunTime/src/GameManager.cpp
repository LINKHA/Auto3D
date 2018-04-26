#include "GameManager.h"



AUTO_BEGIN


GameManager::GameManager()
{
}
GameManager::~GameManager()
{
}




LevelGameManager::LevelGameManager() : Super() { }
LevelGameManager::~LevelGameManager(){}
template<class TransferFunction>
void LevelGameManager::Transfer(TransferFunction& transfer)
{
	Super::Transfer(transfer);
}


GlobalGameManager::GlobalGameManager() : Super() { }
GlobalGameManager::~GlobalGameManager(){}
template<class TransferFunction> 
void GlobalGameManager::Transfer(TransferFunction& transfer)
{
	Super::Transfer(transfer);
}
AUTO_END

