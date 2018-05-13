#ifndef SPRITE_H_
#define SPRITE_H_
#include "BaseObject.h"
#include "stl_use.h"


AUTO_BEGIN
#define GetComponent(x) GetSprite().GetComponentT<x>(ClassID (x))

class Sprite;
class Component;
class Node :public Object
{
	REGISTER_DERIVED_CLASS(Node, Object);
	DECLARE_OBJECT_SERIALIZE(Node);
public:
	Node();
	typedef int arrayIndex;
	typedef _VECTOR(Sprite) SpriteNodeArray;
protected:
	SpriteNodeArray m_Childs;
	UInt32 m_Layer;
	UInt16 m_Tag;
	bool m_IsActive;
public:



	virtual void AddChild(const Sprite& node);
	virtual void RemoveChild(arrayIndex index);
	virtual Sprite GetChild(arrayIndex index);
	virtual SpriteNodeArray GetAllChild();

};



class Component : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Component, Object);
	DECLARE_OBJECT_SERIALIZE(Component);
private:
	Sprite* m_sprite;
public:
	Component();

	Sprite& GetSprite() { return *m_sprite; }
	const Sprite& GetSprite() const { return *m_sprite; }
	Sprite* GetSpritePtr() { return m_sprite; }
	Sprite* GetSpritePtr() const { return m_sprite; }
private:
	friend class Sprite;
};

class Sprite: public Node
{
	REGISTER_DERIVED_CLASS(Sprite, Node);
	DECLARE_OBJECT_SERIALIZE(Sprite);
public:
	typedef AUTO_VECTOR(ClassIDType, Component) ComponentsArray;

protected:
	ComponentsArray m_Components;
public:
	Sprite();
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

	int GetComponentSize() { return m_Components.size(); }

	const Sprite& GetSprite()const	{ return *this; }
	Sprite& GetSprite()				{ return *this; }

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