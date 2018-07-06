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
	m_Prefabs.push_back(prefab);
}
void PreLoadManager::RemovePrefab(Prefab * prefab)
{
	m_Prefabs.remove(prefab);
}
AUTO_END