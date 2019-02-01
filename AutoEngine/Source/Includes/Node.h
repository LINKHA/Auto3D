#pragma once
#include "Object.h"
#include "NodeTrait.h"
#include "AutoMath.h"
#include "AutoSTL.h"

namespace Auto3D {

class Component;
class Transform;
class SceneNode;
class Node;

/// define gameobject with node
using GameNode = SharedPtr<Node>;

class Node :public Object, public EnableSharedFromThis<Node>
{
	REGISTER_OBJECT_CLASS(Node, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit Node(SharedPtr<Ambient> ambient, int sceneID);
	/**
	* @brief : Destory node
	*/
	void Destory();
	/**
	* @brief :Add Child
	* @param : GameObject*
	*/
	virtual void AddChild(SharedPtr<Node> node);
	/**
	* @brief : Mount component in this GameObject
	*/
	void AddComponent(SharedPtr<Component> com);
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
	virtual SharedPtr<Node> GetChild(int index);
	/**
	* @brief : Get this objct all child
	* @return : GameObjectChildArray&
	*/
	virtual VECTOR<SharedPtr<Node> >& GetAllChild();
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
	int GetLevelID() { return _levelID; }

	/**
	* @brief : Get component from index
	*/
	SharedPtr<Component> GetComponentIndex(int index);
	/**
	* @brief : Get components size
	*/
	int GetComponentsSize();
	/**
	* @brief : Get game object
	*/
	SharedPtr<Node> GetNode();
	/**
	* @brief : Get Components
	* @return : PAIR_VECTOR(int, Component*)
	*/
	PAIR_VECTOR<STRING, SharedPtr<Component> >& GetComponentsArray() { return _components; }
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
	* @brief : Set node name
	*/
	void SetName(const STRING& name) { _name = name; }
	/**
	* @brief : Get this object position
	*/
	Vector3 GetPosition();
	/**
	* @brief : Get node name
	*/
	const STRING GetName() const { return _name; }
	/**
	* @brief : Find component from class id
	*/
	SharedPtr<Component> GetComponent(STRING type);
	/**
	* @brief : Get component in _components if nullptr will breaking
	*/
	template<typename _Ty> SharedPtr<_Ty> GetComponent();
	/**
	* @brief : Create component with stirng type
	*/
	SharedPtr<Component> CreateComponent(STRING type);
	/**
	* @brief : Create component with template
	*/
	template<typename _Ty> SharedPtr<_Ty> CreateComponent();
protected:
	SharedPtr<Transform> _transform;
	/// node name
	STRING _name;
	/// node childs (VECTOR(Node*))
	VECTOR<SharedPtr<Node> > _childs;
	/// layer id
	NodeLayout _layer{};
	/// tag id
	NodeTag _tag{};
	/// is active
	bool _isActive;
	/// level id
	int _levelID{};
	/// this gameobject components container
	PAIR_VECTOR<STRING, SharedPtr<Component> > _components;
	/// enable
	bool _isEnable;
};

template<typename _Ty> SharedPtr<_Ty> Node::CreateComponent()
{
	return StaticCast<_Ty>(CreateComponent(_Ty::GetClassStringStatic()));
}

template<typename _Ty> SharedPtr<_Ty> Node::GetComponent()
{
	return StaticCast<_Ty>(GetComponent(_Ty::GetClassStringStatic()));
}



}