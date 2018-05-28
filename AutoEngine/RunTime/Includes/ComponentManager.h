#ifndef COMPONENT_MANAGER_H_
#define COMPONENT_MANAGER_H_

#include "GameManager.h"
#include "GameObject.h"
#include "Singleton.h"

AUTO_BEGIN
class Component;

class ComponentManager : public LevelGameManager,public Singleton<ComponentManager>
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ComponentManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(ComponentManager);
	typedef _LIST(Component*)	ComponentList;

public:
	ComponentManager();
	void AddComponent(Component * c);
	void RemoveComponent(Component * c);

	void UpdateComponent();

private:
	void DelayAddRemoveComponent();
private:
	ComponentList	m_Compontents;
	ComponentList	m_CompontentsToAdd;
	ComponentList	m_CompontentsToRemove;
	//Update or Start now
	bool			m_InsideRun;
};
AUTO_END
#endif // !COMPONENT_MANAGER_H_