#pragma once
#include "GameManager.h"
#include "Singleton.h"
#include "stl_use.h"
#include "Prefab.h"
namespace Auto3D {
class PreLoadManager : public GlobalGameManager , public Singleton<PreLoadManager>
{
	REGISTER_DERIVED_CLASS(PreLoadManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(PreLoadManager);
	typedef _LIST(Prefab*)	PreContainer;
public:
	PreLoadManager();
	void AddPrefab(Prefab* prefab);
	void RemovePrefab(Prefab* prefab);
	PreContainer GetPrefabs() { return _prefabs; }
private:
	PreContainer _prefabs;
};
}

