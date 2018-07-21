#include "Prefab.h"
#include "PreLoadManager.h"
AUTO_BEGIN

Prefab::Prefab(GameObject* gameObject)
	:_gameObject(gameObject)
{
	INSTANCE(PreLoadManager).AddPrefab(this);
}
Prefab::Prefab(GameObject& gameObject)
	: _gameObject(&gameObject)
{
}

Prefab::~Prefab()
{}


AUTO_END