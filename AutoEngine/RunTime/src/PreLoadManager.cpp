#include "PreLoadManager.h"

AUTO_BEGIN

SINGLETON_INSTANCE(PreLoadManager);

PreLoadManager::PreLoadManager()
{
}


PreLoadManager::~PreLoadManager()
{
}

void PreLoadManager::AddPrefab(Prefab * prefab)
{
	_prefabs.push_back(prefab);
}
void PreLoadManager::RemovePrefab(Prefab * prefab)
{
	_prefabs.remove(prefab);
}
AUTO_END