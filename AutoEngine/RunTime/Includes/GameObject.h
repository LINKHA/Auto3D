#pragma once
#include "Object.h"
#include "stl_use.h"
#include "ComponentSetting.h"

namespace Auto3D {
#define GetComponent(x) GetComponentT<x>(ClassID (x))
class GameObject;
class Transform;
class Camera;
class Ambient;
class Node :public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Node, Object);
	DECLARE_OBJECT_SERIALIZE(Node);
	using GameObjectNodeArray = _VECTOR(GameObject);
public:
	Node() {}
	explicit Node(Ambient* ambient);
	virtual void AddChild(const GameObject& node);
	virtual void RemoveChild(int index);
	virtual GameObject& GetChild(int index);
	virtual GameObjectNodeArray& GetAllChild();

protected:
	GameObjectNodeArray _childs;
	UInt32 _layer;
	UInt16 _tag;
	bool _isActive;
};

class Component : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Component, Object);
	DECLARE_OBJECT_SERIALIZE(Component);
private:
	Ptr(GameObject, _gameObject);
public:
	Component();
	explicit Component(Ambient* ambient);
	GameObject& GetGameObject();
	const GameObject& GetGameObject() const;
	GameObject* GetGameObjectPtr();
	GameObject* GetGameObjectPtr() const;

	void MountComponent(GameObject& gameObject);

	void Enable(bool enable) { _isEnable = enable; }
	bool GetEnable() { return _isEnable; }

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
private:
	bool _isEnable;
};

class GameObject : public Node
{
	REGISTER_DERIVED_CLASS(GameObject, Node);
	DECLARE_OBJECT_SERIALIZE(GameObject);
public:
	using ComponentsArray = AUTO_VECTOR(int, Component*);

public:
	GameObject();
	/**
	* @brief : Register game object to GameObjectManager
	*/
	explicit GameObject(Ambient* ambient);
	void Destory();
	/**
	* @brief : Set game object enable
	*/
	void Enable(bool enable) { _isEnable = enable; }
	/**
	* @brief : Get game object enable
	*/
	bool GetEnable() { return _isEnable; }

	//void SetLayer(int layerIndex);
	//int GetLayer() const { return m_Layer; }

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
	template<class T> inline T& GetComponentT(int compareClassID) const;
	/**
	* @brief : Get component from index
	*/
	inline  Component& GetComponentIndex(int index);
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
	* @brief : Get Components (AUTO_VECTOR(int, Component*))
	*/
	ComponentsArray& GetComponentsArray(){ return _components; }

private:
	ComponentsArray _components;
	bool _isEnable;
};

template<class T> inline T& GameObject::GetComponentT(int compareClassID) const
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
