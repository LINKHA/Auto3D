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
	GameObjectNodeArray m_Childs;
	UInt32 m_Layer;
	UInt16 m_Tag;
	bool m_IsActive;
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
	Ptr(GameObject, m_gameObject);
public:
	Component();
	GameObject& GetGameObject();
	const GameObject& GetGameObject() const;
	GameObject* GetGameObjectPtr();
	GameObject* GetGameObjectPtr() const;

	void MountComponent(GameObject& gameObject);

	void Enable(bool enable) { m_Enable = enable; }
	bool GetEnable() { return m_Enable; }

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw(Camera* camera=nullptr){}
private:
	bool m_Enable;
};

class GameObject : public Node
{
	REGISTER_DERIVED_CLASS(GameObject, Node);
	DECLARE_OBJECT_SERIALIZE(GameObject);
public:
	typedef AUTO_VECTOR(int, Component*) ComponentsArray;

private:
	ComponentsArray m_Components;
public:
	GameObject();
	GameObject(Transform* transform);
	void Enable(bool enable) { m_Enable = enable; }
	bool GetEnable() { return m_Enable; }
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
	ComponentsArray& GetComponentsArray(){ return m_Components; }

private:
	bool m_Enable;
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
	return *m_Components[index].second;
}
AUTO_END
