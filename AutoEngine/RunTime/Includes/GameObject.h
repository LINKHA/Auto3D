#ifndef SPRITE_H_
#define SPRITE_H_
#include "BaseObject.h"
#include "stl_use.h"
#include "Transform.h"
AUTO_BEGIN
#define GetComponent(x) GetGameObject().GetComponentT<x>(ClassID (x))
class GameObject;
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

class GameObject;

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
	GameObject* GetGameObjectePtr();
	GameObject* GetGameObjectPtr() const;

	void MountComponent(GameObject& gameObject);
};

class GameObject : public Node
{
	REGISTER_DERIVED_CLASS(GameObject, Node);
	DECLARE_OBJECT_SERIALIZE(GameObject);
public:
	typedef AUTO_VECTOR(ClassIDType, Component) ComponentsArray;

private:
	ComponentsArray m_Components;
	Ptr(Transform, m_Transform);
public:
	GameObject();
	void Enable();
	void Destory();

	//void SetLayer(int layerIndex);
	//int GetLayer() const { return m_Layer; }

	void AddComponent(Component& com);
	void RemoveComponentAtIndex(int index);
	template<class T> inline T& GetComponentT(int compareClassID) const;
	inline  Component& GetComponentIndex(int index);
	int GetComponentSize();

	const GameObject& GetGameObject()const;
	GameObject& GetGameObject();

	Component QueryComponent(int classID) const;
	Transform& GetTransform()const;
	Transform * GetTransformPtr()const;
};

template<class T> inline
T& GameObject::GetComponentT(int compareClassID) const
{
	Component* com;
	com = QueryComponent(compareClassID);
	AssertIf(com == NULL);
	return *static_cast<T*> (com);
}
AUTO_END
#endif // SPRITE_H_