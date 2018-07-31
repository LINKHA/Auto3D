#pragma once
#include "Object.h"
#include "stl_use.h"

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
	virtual void Draw(){}
private:
	bool _isEnable;
};

class GameObject : public Node
{
	REGISTER_DERIVED_CLASS(GameObject, Node);
	DECLARE_OBJECT_SERIALIZE(GameObject);
public:
	using ComponentsArray = AUTO_VECTOR(int, Component*);

private:
	ComponentsArray _components;
public:
	GameObject();
	explicit GameObject(Ambient* ambient);
	void Enable(bool enable) { _isEnable = enable; }
	bool GetEnable() { return _isEnable; }
	void Destory();

	//void SetLayer(int layerIndex);
	//int GetLayer() const { return m_Layer; }

	void AddComponent(Component* com);
	void RemoveComponentAtIndex(int index);
	template<class T> inline T& GetComponentT(int compareClassID) const;
	inline  Component& GetComponentIndex(int index);
	int GetComponentSize();

	const GameObject& GetGameObject()const;
	GameObject& GetGameObject();

	Component* QueryComponent(int classID) const;
	ComponentsArray& GetComponentsArray(){ return _components; }

private:
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
