#pragma once
#include "Object.h"
#include "stl_use.h"
#include "ComponentSetting.h"
#include "Math/Vector3.h"
#include "../../EngineSetting/GameSetting.h"
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
	using GameObjectChilds = _VECTOR(GameObject);
public:
	explicit Node(Ambient* ambient,int levelBumber);
	/**
	* @brief :Add Child
	* @param : GameObject&
	*/
	virtual void AddChild(const GameObject& node);
	/**
	* @brief : Remove child with index
	*/
	virtual void RemoveChild(int index);
	/**
	* @brief : Get this objct child with index
	* @return :GameObject&
	*/
	virtual GameObject& GetChild(int index);
	/**
	* @brief : Get this objct all child
	* @return : GameObjectChildArray&
	*/
	virtual GameObjectChilds& GetAllChild();
	/**
	* @brief : Set layer clamp(0~layer count)
	*/
	void SetLayer(Layout layer) { _layer = clamp(static_cast<UInt32>(layer), static_cast<UInt32>(0), static_cast<UInt32>(Layout::klayoutCount));}
	/**
	* @brief : Set tag clamp(0~tag count)
	*/
	void SetTag(Tag tag){ _layer = clamp(static_cast<UInt16>(tag), static_cast<UInt16>(0), static_cast<UInt16>(Tag::kTagCount)); }
	/**
	* @brief : Get layer
	* @return; enum Layout
	*/
	Layout GetLayer() const { return static_cast<Layout>(_layer); }
	/**
	* @brief : Get tag
	* @return; enum Tag
	*/
	Tag GetTag() const { return static_cast<Tag>(_tag); }
protected:
	GameObjectChilds _childs;
	UInt32 _layer{};
	UInt16 _tag{};
	bool _isActive;
	int _levelBumber{};
};

class Component : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Component, Object);
	DECLARE_OBJECT_SERIALIZE(Component);
private:
	SharedPtr<GameObject> _gameObject;
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
	/**
	* @brief : Get this object position
	*/
	Vector3 GetPosition(); 

private:
	ComponentsArray _components;
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
