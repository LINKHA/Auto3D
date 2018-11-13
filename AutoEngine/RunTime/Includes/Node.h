#pragma once
#include "Object.h"
#include "../../EngineSetting/GameSetting.h"
#include "Math/Math.h"


namespace Auto3D {
class Component;
class Transform;

//#define GetComponent(x) GetComponentT<x>(ClassID (x))
/// define gameobject with node
using GameObject = class Node;

class Node :public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Node, Object);
	DECLARE_OBJECT_SERIALIZE(Node);
	using GameObjectChilds = VECTOR(Node*);
#if SharedPtrDebug
	using ComponentsArray = PAIR_VECTOR(STRING, SharedPtr<Component>);
#else
	using ComponentsArray = PAIR_VECTOR(STRING, Component*);
#endif
public:
	explicit Node(Ambient* ambient, int sceneID);
	/**
	* @brief : Get scene ID
	*/
	int GetSceneID() { return _sceneID; }
	/**
	* @brief :Add Child
	* @param : GameObject*
	*/
	virtual void AddChild(Node* node);
	/**
	* @brief : Remove child with index
	*/
	virtual void RemoveChild(int index);
	/**
	* @brief : Get this objct child with index
	* @return :GameObject*
	*/
	virtual Node* GetChild(int index);
	/**
	* @brief : Get this objct all child
	* @return : GameObjectChildArray&
	*/
	virtual GameObjectChilds& GetAllChild();
	/**
	* @brief : Set layer clamp(0~layer count)
	*/
	void SetLayer(Layout layer) { _layer = clamp(static_cast<UInt32>(layer), static_cast<UInt32>(0), static_cast<UInt32>(Layout::klayoutCount)); }
	/**
	* @brief : Set tag clamp(0~tag count)
	*/
	void SetTag(Tag tag) { _layer = clamp(static_cast<UInt16>(tag), static_cast<UInt16>(0), static_cast<UInt16>(Tag::kTagCount)); }
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
	const Node& GetGameObject()const;
	/**
	* @brief : Get game object
	*/
	Node& GetGameObject();
	/**
	* @brief : Get Components
	* @return : PAIR_VECTOR(int, Component*)
	*/
	ComponentsArray& GetComponentsArray() { return _components; }
	/**
	* @brief : Get this object position
	*/
	Vector3 GetPosition();
	/**
	* @brief : Find component from class id
	*/
	Component* GetComponent(STRING type);
	/**
	* @brief : Get component in _components if nullptr will breaking
	*/
	template<typename _Ty> _Ty* GetComponent();
	

	Component* CreateComponent(STRING type);

	template <typename _Ty> _Ty* CreateComponent();
protected:
	GameObjectChilds _childs;
	UInt32 _layer{};
	UInt16 _tag{};
	bool _isActive;
	int _sceneID{};

	/// this gameobject components container
	ComponentsArray _components;
	/// enable
	bool _isEnable;
};

template <typename _Ty> _Ty* Node::CreateComponent()
{
	return static_cast<_Ty*>(CreateComponent(_Ty::GetClassStringStatic()));
}

template<typename _Ty> _Ty* Node::GetComponent()
{
	return static_cast<_Ty*>(GetComponent(_Ty::GetClassStringStatic()));
}

inline Component& Node::GetComponentIndex(int index)
{
	return *_components[index].second;
}


}