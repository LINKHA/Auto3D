#pragma once
#include "Component.h"
#include "AutoPhysics2D.h"

namespace Auto3D {

class RigidBody2D;

class Collider2D : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Collider2D, Component)
public:
	/**
	* @brief : Construct
	*/
	explicit Collider2D(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Create fixture
	*/
	void CreateFixture();
	/**
	* @brief : Release fixture
	*/
	void ReleaseFixture();
	/**
	* @brief : Set trigger
	*/
	void SetTrigger(bool trigger);
	/**
	* @brief : Set filter category bits
	*/
	void SetCategoryBits(int categoryBits);
	/**
	* @brief : Set filter mask bits
	*/
	void SetMaskBits(int maskBits);
	/**
	* @brief : Set filter group index
	*/
	void SetGroupIndex(int groupIndex);
	/**
	* @brief : Set density
	*/
	void SetDensity(float density);
	/**
	* @brief : Set friction
	*/
	void SetFriction(float friction);
	/**
	* @brief : Set restitution 
	*/
	void SetRestitution(float restitution);
	/**
	* @brief : Return filter category bits
	*/
	int GetCategoryBits() const { return _fixtureDef.filter.categoryBits; }
	/**
	* @brief : Return filter mask bits
	*/
	int GetMaskBits() const { return _fixtureDef.filter.maskBits; }
	/**
	* @brief : Return filter group index
	*/
	int GetGroupIndex() const { return _fixtureDef.filter.groupIndex; }
	/**
	* @brief : Return density
	*/
	float GetDensity() const { return _fixtureDef.density; }
	/**
	* @brief : Return friction
	*/
	float GetFriction() const { return _fixtureDef.friction; }
	/**
	* @brief : Return restitution
	*/
	float GetRestitution() const { return _fixtureDef.restitution; }
	/**
	* @brief : Return mass
	*/
	float GetMass() const;
	/**
	* @brief : Return inertia
	*/
	float GetInertia() const;
	/**
	* @brief : Return mass center
	*/
	Vector2 GetMassCenter() const;
	/**
	* @brief : Return fixture
	*/
	b2Fixture* GetFixture() const { return _fixture; }
protected:
	/// Fixture def
	b2FixtureDef _fixtureDef;
	/// Box2D fixture
	b2Fixture* _fixture;
	/// Rigbody
	SharedPtr<RigidBody2D> _rigidBody;
	/// Cached world scale.
	Vector3 _cachedWorldScale;
};

}