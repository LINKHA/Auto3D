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
	// Remove node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_levelBumber)->RemoveNode(this);
}

void GameObject::AddComponent(Component* com)
{
	_components.PushBack(KhSTL::MakePair(com->GetClassID(), com));
	//_com.push_back(std::make_pair(com->GetClassID(), com));

	com->MountComponent(*this);
}

void GameObject::RemoveComponentAtIndex(int index)
{
	ComponentsArray::Iterator it = _components.Begin() + index;
	_components.Erase(it);
}

int GameObject::GetComponentsSize() 
{ 
	return static_cast<int>(_components.Size());
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
	for (auto it = _components.Begin(); it != _components.End(); it++)
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