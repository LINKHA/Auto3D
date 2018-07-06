#ifndef PRE_LOAD_MANAGER_H_
#define PRE_LOAD_MANAGER_H_
#include "GameManager.h"
#include "Singleton.h"
#include "stl_use.h"
#include "Prefab.h"
AUTO_BEGIN
class PreLoadManager : public GlobalGameManager , public Singleton<PreLoadManager>
{
	REGISTER_DERIVED_CLASS(PreLoadManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(PreLoadManager);
	typedef _LIST(Prefab*)	PreContainer;
public:
	PreLoadManager();
	void AddPrefab(Prefab * prefab);
	void RemovePrefab(Prefab * prefab);
	PreContainer GetPrefabs() { return m_Prefabs; }
private:
	PreContainer m_Prefabs;
};
AUTO_END
#endif //!PRE_LOAD_MANAGER_H_
