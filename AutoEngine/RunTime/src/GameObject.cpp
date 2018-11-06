#include "GameObject.h"
#include "Transform.h"
#include "Ambient.h"
#include "Scene.h"
#include "LevelScene.h"
#include "Ambient.h"
#include "Component.h"


namespace Auto3D {

GameObject::GameObject(Ambient* ambient,int levelBumber)
	: Super(ambient, levelBumber)
	,_isEnable(true)
{
	AddComponent(_ambient->CreateObject<Transform>());
	// add node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_levelBumber)->AddNode(this);
}
GameObject::~GameObject()
{
	// remove node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_levelBumber)->RemoveNode(this);
}

void GameObject::AddComponent(Component* com)
{
	_components.push_back(MAKE_PAIR(com->GetClassID(), com));
	com->MountComponent(*this);
}

void GameObject::RemoveComponentAtIndex(int index)
{
	ComponentsArray::iterator it = _components.begin() + index;
	_components.erase(it);
}

int GameObject::GetComponentsSize() 
{ 
	return static_cast<int>(_components.size());
}

const GameObject& GameObject::GetGameObject()const 
{ 
	return *this; 
}

GameObject& GameObject::GetGameObject() 
{ 
	return *this; 
}

Component* GameObject::QueryComponent(int classID) const
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		if (it->first == classID)
			return it->second;
	}
	ErrorString("File find component of ClassId.");
	return nullptr;
}

Vector3 GameObject::GetPosition()
{
	return GetComponent(Transform).GetPosition();
}




}