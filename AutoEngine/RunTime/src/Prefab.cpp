#include "Prefab.h"
#include "PreLoadManager.h"
AUTO_BEGIN

Prefab::Prefab(GameObject * gameObject)
	:m_GameObject(gameObject)
{
	INSTANCE(PreLoadManager).AddPrefab(this);
}
Prefab::Prefab(GameObject& gameObject)
	: m_GameObject(&gameObject)
{
}

Prefab::~Prefab()
{}


AUTO_END