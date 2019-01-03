#pragma once
#include "GameManager.h"
#include "Prefab.h"
namespace Auto3D {
class PreLoadManager : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(PreLoadManager, GlobalGameManager)

	using PreContainer = LIST<Prefab*>;
public:
	explicit PreLoadManager(SharedPtr<Ambient> ambient);
	void AddPrefab(Prefab* prefab);
	void RemovePrefab(Prefab* prefab);
	PreContainer GetPrefabs() { return _prefabs; }
private:
	PreContainer _prefabs;
};
}

