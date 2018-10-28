#pragma once
#include "Object.h"
#include "../../EngineSetting/GameSetting.h"
#include "Math/Math.h"

namespace Auto3D {
class GameObject;

class Node :public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Node, Object);
	DECLARE_OBJECT_SERIALIZE(Node);
	using GameObjectChilds = VECTOR(GameObject*);
public:
	explicit Node(Ambient* ambient, int levelBumber);
	/**
	* @brief :Add Child
	* @param : GameObject*
	*/
	virtual void AddChild(GameObject* node);
	/**
	* @brief : Remove child with index
	*/
	virtual void RemoveChild(int index);
	/**
	* @brief : Get this objct child with index
	* @return :GameObject*
	*/
	virtual GameObject* GetChild(int index);
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
protected:
	GameObjectChilds _childs;
	UInt32 _layer{};
	UInt16 _tag{};
	bool _isActive;
	int _levelBumber{};
};

}