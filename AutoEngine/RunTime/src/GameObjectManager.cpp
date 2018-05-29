#include "GameObjectManager.h"

AUTO_BEGIN

SINGLETON_INSTANCE(GameObjectManager);

GameObjectManager::GameObjectManager()
{
}
GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::AddGameObject(GameObject * obj)
{
	Assert(obj != NULL);
	if (m_InsideRun)
	{
		m_GameObjectsToRemove.remove(obj);
		m_GameObjectsToAdd.push_back(obj);
		return;
	}
	m_GameObjectsToAdd.remove(obj);
	m_GameObjectsToRemove.remove(obj);
	m_GameObjects.push_back(obj);
}

void GameObjectManager::RemoveGameObject(GameObject * obj)
{
	Assert(obj != NULL);
	m_GameObjectsToAdd.remove(obj);
	m_GameObjectsToRemove.remove(obj);
	if (m_InsideRun)
	{
		m_GameObjectsToRemove.push_back(obj);
	}
	else
	{
		m_GameObjects.remove(obj);
	}
}
void GameObjectManager::ModeRunGameObject(GameObjectRunMode runMode)
{
	if (runMode == DefaultMode)
	{
		ErrorString("GameObejct fail to Run.");
		return;
	}
	m_InsideRun = true;
	for (GameObjectContainer::iterator i = m_GameObjects.begin(); i != m_GameObjects.end(); i++)
	{
		GameObject* obj = *i;
		if (obj && obj->GetEnable())
		{
#define TEMP_ITERATOR for (AUTO_VECTOR(int, Component*)::iterator i = obj->GetComponentsArray().begin(); i != obj->GetComponentsArray().end(); i++)
			if (runMode == AwakeMode) 
				TEMP_ITERATOR{ if (i->second->GetEnable()) i->second->Awake();}
			else if (runMode == StartMode)
				TEMP_ITERATOR{ if (i->second->GetEnable()) i->second->Start(); }
			else if (runMode == UpdateMode)
				TEMP_ITERATOR{ if (i->second->GetEnable()) i->second->Update(); }
			else if (runMode == FixUpdateMode)
				TEMP_ITERATOR{ if (i->second->GetEnable()) i->second->FixUpdate(); }
			else if (runMode == FinishMode)
				TEMP_ITERATOR{ if (i->second->GetEnable()) i->second->Finish(); }
			else 
				ErrorString("GameObejct fail to Run.");
#undef TEMP_ITERATOR
		}
	}
	m_InsideRun = false;
	DelayAddRemoveGameObject();
}


///Private
void GameObjectManager::DelayAddRemoveGameObject()
{
	DebugAssertIf(m_InsideRun);
	for (GameObjectContainer::iterator i = m_GameObjectsToRemove.begin(); i != m_GameObjectsToRemove.end(); /**/)
	{
		GameObject* obj = *i;
		++i; 
		RemoveGameObject(obj);
	}
	m_GameObjectsToRemove.clear();
	for (GameObjectContainer::iterator i = m_GameObjectsToAdd.begin(); i != m_GameObjectsToAdd.end(); /**/)
	{
		GameObject* obj = *i;
		++i; 
		AddGameObject(obj);
	}
	m_GameObjectsToAdd.clear();
}


AUTO_END