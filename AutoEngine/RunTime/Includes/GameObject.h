#pragma once

#include <vector>

#include "Object.h"
#include "stl_use.h"
#include "ComponentSetting.h"
#include "Math/Vector3.h"
#include "../../EngineSetting/GameSetting.h"
#include "Node.h"


namespace Auto3D {
class Component;
class Transform;

#define GetComponent(x) GetComponentT<x>(ClassID (x))


class GameObject : public Node
{
	REGISTER_DERIVED_CLASS(GameObject, Node);
	DECLARE_OBJECT_SERIALIZE(GameObject);
public:
#if SharedPtrDebug
	using ComponentsArray = AUTO_VECTOR(int, SharedPtr<Component>);
#else
	using ComponentsArray = AUTO_VECTOR(int, Component*);
#endif

public:
	/**
	* @brief : Register game object to GameObjectManager
	*/
	explicit GameObject(Ambient* ambient, int levelBumber);

	void Destory();
	/**
	* @brief : Set game object enable
	*/
	void Enable(bool enable) { _isEnable = enable; }
	/**
	* @brief : Get game object enable
	*/
	bool GetEnable() { return _isEnable; }
	/**
	* @brief : Mount component in this GameObject
	*/
	void AddComponent(Component* com);
	/**
	* @brief : Remove component at index
	*/
	void RemoveComponentAtIndex(int index);
	/**
	* @brief : Get component in _components if nullptr will breaking
	*/
	template<typename T> inline T& GetComponentT(int compareClassID) const;
	/**
	* @brief : Get component from index
	*/
	inline Component& GetComponentIndex(int index);
	/**
	* @brief : Get components size
	*/
	int GetComponentsSize();
	/**
	* @brief : Get game object 
	*/
	const GameObject& GetGameObject()const;
	/**
	* @brief : Get game object
	*/
	GameObject& GetGameObject();
	/**
	* @brief : Find component from class id
	*/
	Component* QueryComponent(int classID) const;
	/**
	* @brief : Get Components 
	* @return : AUTO_VECTOR(int, Component*)
	*/
	ComponentsArray& GetComponentsArray(){ return _components; }
	/**
	* @brief : Get this object position
	*/
	Vector3 GetPosition(); 

private:
	/// this gameobject components container
	ComponentsArray _components;

	//std::vector<std::pair<int, Component*>> _com;
	/// enable
	bool _isEnable;
};

template<typename T> inline T& GameObject::GetComponentT(int compareClassID) const
{
	Component* com;
	com = QueryComponent(compareClassID);
	Assert(com != nullptr);
	return *static_cast<T*> (com);
}

inline Component& GameObject::GetComponentIndex(int index)
{
	return *_components[index].second;
}

}
