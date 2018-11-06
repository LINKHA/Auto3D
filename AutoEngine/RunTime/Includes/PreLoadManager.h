#pragma once
#include "GameManager.h"
#include "Singleton.h"
#include "AutoSTL.h"
#include "Prefab.h"
namespace Auto3D {
class PreLoadManager : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(PreLoadManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(PreLoadManager);
	typedef LIST(Prefab*)	PreContainer;
public:
	explicit PreLoadManager(Ambient* ambient);
	void AddPrefab(Prefab* prefab);
	void RemovePrefab(Prefab* prefab);
	PreContainer GetPrefabs() { return _prefabs; }
private:
	PreContainer _prefabs;
};
}

