#pragma once
#include "Object.h"
#include "GameObject.h"

namespace Auto3D {

const static char* SCENE_ATTACH = "Scene_attach";
const static char* GEOMETRY_ATTACH = "GeoMetry_attach";
const static char* COMPONENT_SET_ATTACH = "Component_Set_attach";


class Component : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Component, Object);
	DECLARE_OBJECT_SERIALIZE(Component);

public:
	explicit Component(Ambient* ambient);
	/**
	* @brief : Get game object quote
	*/
	GameObject& GetGameObject();
	/**
	* @brief : Get game object quote const
	*/
	const GameObject& GetGameObject() const;
	/**
	* @brief : Get game object ptr
	*/
	GameObject* GetGameObjectPtr();
	/**
	* @brief : Get game object ptr const
	*/
	GameObject* GetGameObjectPtr() const;
	/**
	* @brief : Mount component for gameobject
	*/
	void MountComponent(GameObject& gameObject);
	/**
	* @brief : Set enable
	*/
	void Enable(bool enable) { _isEnable = enable; }
	/**
	* @brief : Return enable
	*/
	bool GetEnable() { return _isEnable; }

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
private:
	SharedPtr<GameObject> _gameObject;
	bool _isEnable;
};

}