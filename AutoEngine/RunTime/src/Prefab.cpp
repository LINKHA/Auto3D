#include "Prefab.h"
#include "PreLoadManager.h"
namespace Auto3D {

Prefab::Prefab(Ambient* ambient)
	:Super(ambient)
{
	INSTANCE(PreLoadManager).AddPrefab(this);
}


Prefab::~Prefab()
{}


}