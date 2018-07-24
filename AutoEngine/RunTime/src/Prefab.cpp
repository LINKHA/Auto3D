#include "Prefab.h"
#include "PreLoadManager.h"
AUTO_BEGIN

Prefab::Prefab(Ambient* ambient)
	:Super(ambient)
{
	INSTANCE(PreLoadManager).AddPrefab(this);
}


Prefab::~Prefab()
{}


AUTO_END