#pragma once
#include "Object.h"
#include "NodeLayout.h"
#include "NodeTag.h"
#include "Math/Math.h"


namespace Auto3D {

class Component;
class Transform;
class SceneNode;

/// define gameobject with node
using GameObject = class Node;

class Node :public Object
{
	REGISTER_DERIVED_CLASS(Node, Object);
	DECLARE_OBJECT_SERIALIZE(Node);
	using NodeChilds = VECTOR<Node*>;
#if SharedPtrDebug
	using ComponentsArray = PAIR_VECTOR(STRING, sharedPtr<Component>);
#else
	using ComponentsArray = PAIR_VECTOR<STRING, Component*>;
#endif
public:
	explicit Node(Ambient* ambient, int sceneID);
	/**
	* @brief :Add Child
	* @param : GameObject*
	*/
	virtual void AddChild(Node* node);
	/**
	* @brief : Mount component in this GameObject
	*/
	void AddComponent(Component* com);
	/**
	* @brief : Remove component at index
	*/
	void RemoveComponentAtIndex(int index);
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
	virtual NodeChilds& GetAllChild();
	/**
	* @brief : Set layer
	*/
	void SetLayer(const NodeLayout& layer) { _layer = layer; }
	/**
	* @brief : Set tag
	*/
	void SetTag(const NodeTag& tag) { _tag = tag; }
	/**
	* @brief : Get layer
	*/
	NodeLayout GetLayer() const { return _layer; }
	/**
	* @brief : Get tag
	*/
	NodeTag GetTag() const { return _tag; }
	/**
	* @brief : Get game object enable
	*/
	bool GetEnable() { return _isEnable; }
	/**
	* @brief : Get scene ID
	*/
	int GetSceneID() { return _sceneID; }

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
	const Node& GetNode()const;
	/**
	* @brief : Get game object
	*/
	Node& GetNode();
	/**
	* @brief : Get Components
	* @return : PAIR_VECTOR(int, Component*)
	*/
	ComponentsArray& GetComponentsArray() { return _components; }
	/**
	* @brief : Set position from vector2
	*/
	void SetPosition(const Vector2& position);
	/**
	* @brief : Set position from vector3
	*/
	void SetPosition(const Vector3& position);
	/**
	* @brief : Set position from x,y
	*/
	void SetPosition(float x, float y);
	/**
	* @brief : Set position from x,y,z
	*/
	void SetPosition(float x,float y,float z);
	/**
	* @brief : Set Rotation from rotation
	*/
	void SetRotation(const Quaternion& rotation);
	/**
	* @brief : Set Rotation from euler(Vector3)
	*/
	void SetRotation(const Vector3& euler);
	/**
	* @brief : Set Rotation from angle and axis
	*/
	void SetRotation(float Angle, const Vector3& axis);
	/**
	* @brief : Set position from scale
	*/
	void SetScale(float scale);
	/**
	* @brief : Set position from scaleX,scaleY
	*/
	void SetScale(float scaleX, float scaleY);
	/**
	* @brief : Set position from scaleX,scaleY,scaleZ
	*/
	void SetScale(float scaleX, float scaleY, float scaleZ);
	/**
	* @brief : Set position from scaleX,scaleY,scaleZ
	*/
	void SetScale(const Vector2& scale);
	/**
	* @brief : Set position from scale
	*/
	void SetScale(const Vector3& scale);
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
	/**
	* @brief : Create component with stirng type
	*/
	Component* CreateComponent(STRING type);
	/**
	* @brief : Create component with template
	*/
	template <typename _Ty> _Ty* CreateComponent();
protected:
	/// node childs (VECTOR(Node*))
	NodeChilds _childs;
	/// layer id
	NodeLayout _layer{};
	/// tag id
	NodeTag _tag{};
	/// is active
	bool _isActive;
	/// scene id
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