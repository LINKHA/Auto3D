#ifndef SPRITE_H_
#define SPRITE_H_
#include "BaseObject.h"
#include "stl_use.h"

AUTO_BEGIN
#define GetComponent(x) GetGameObject().GetComponentT<x>(ClassID (x))

class GameObject;
class Node :public Object
{
	REGISTER_DERIVED_CLASS(Node, Object);
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
	GameObject* m_GameObject;
public:
	Component();

	GameObject& GetGameObject() { return *m_GameObject; }
	const GameObject& GetGameObject() const { return *m_GameObject; }
	GameObject* GetGameObjectePtr() { return m_GameObject; }
	GameObject* GetGameObjectPtr() const { return m_GameObject; }

	void MountComponent(GameObject* gameObject, Component& com);
	void MountComponent(GameObject* gameObject, Component* com);

private:
	friend class GameObject;
};

class GameObject: public Node
{
	REGISTER_DERIVED_CLASS(GameObject, Node);
	DECLARE_OBJECT_SERIALIZE(GameObject);
public:
	typedef AUTO_VECTOR(ClassIDType, Component) ComponentsArray;

protected:
	ComponentsArray m_Components;
public:
	GameObject();
	void Enable();
	void Destory();

	void SetLayer(int layerIndex);
	int GetLayer() const { return m_Layer; }

	void AddComponent(const Component& com)
	{
		m_Components.push_back(M_PAIR(com.GetClassID(), com));
	}
	void RemoveComponentAtIndex(int index)
	{
		ComponentsArray::iterator it = m_Components.begin() + index;

		m_Components.erase(it);


	}
	template<class T> inline
	T& GetComponentT(int compareClassID) const
	{
		Component* com;
		com = QueryComponent(compareClassID);
		AssertIf(com == NULL);
		return *static_cast<T*> (com);
	}

	inline  Component& GetComponentIndex(int index)
	{
		return m_Components[index].second;
	}

	int GetComponentSize() { return (int)m_Components.size(); }

	const GameObject& GetGameObject()const	{ return *this; }
	GameObject& GetGameObject()				{ return *this; }

	Component QueryComponent(int classID) const
	{
		for (auto it = m_Components.begin(); it != m_Components.end(); it++)
		{
			if (it->first == classID)
				return it->second;
		}
		ErrorString("File find component of ClassId.");
		//nullptr
		return *(Component*)NULL;
	}

private:
	friend class Component;
};

AUTO_END
#endif // SPRITE_H_