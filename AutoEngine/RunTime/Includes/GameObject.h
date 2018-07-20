#pragma once
#include "BaseObject.h"
#include "stl_use.h"

AUTO_BEGIN
#define GetComponent(x) GetComponentT<x>(ClassID (x))
class GameObject;
class Transform;
class Camera;
class Node :public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Node, Object);
	DECLARE_OBJECT_SERIALIZE(Node);
public:
	Node();
	typedef int arrayIndex;
	typedef _VECTOR(GameObject) GameObjectNodeArray;
protected:
	GameObjectNodeArray _childs;
	UInt32 _layer;
	UInt16 _tag;
	bool _isActive;
public:
	virtual void AddChild(const GameObject& node);
	virtual void RemoveChild(arrayIndex index);
	virtual GameObject GetChild(arrayIndex index);
	virtual GameObjectNodeArray GetAllChild();

};

class Component : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Component, Object);
	DECLARE_OBJECT_SERIALIZE(Component);
private:
	Ptr(GameObject, _gameObject);
public:
	Component();
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
	typedef AUTO_VECTOR(int, Component*) ComponentsArray;

private:
	ComponentsArray _components;
public:
	GameObject();
	GameObject(Transform* transform);
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
	AssertIf(com == NULL);
	return *static_cast<T*> (com);
}
inline Component& GameObject::GetComponentIndex(int index)
{
	return *_components[index].second;
}
AUTO_END
