#include "PreLoadManager.h"
#include "NewDef.h"

namespace Auto3D {

PreLoadManager::PreLoadManager(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


PreLoadManager::~PreLoadManager()
{
}

void PreLoadManager::AddPrefab(Prefab* prefab)
{
	_prefabs.push_back(prefab);
}
void PreLoadManager::RemovePrefab(Prefab* prefab)
{
	_prefabs.remove(prefab);
}
}