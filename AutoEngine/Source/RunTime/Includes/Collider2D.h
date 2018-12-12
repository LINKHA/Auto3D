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
	explicit Collider2D(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;

	void CreateFixture();

	void ReleaseFixture();
	/**
	* @brief : Set trigger
	*/
	void SetTrigger(bool trigger);
	/// Set filter category bits.
	void SetCategoryBits(int categoryBits);
	/// Set filter mask bits.
	void SetMaskBits(int maskBits);
	/// Set filter group index.
	void SetGroupIndex(int groupIndex);
	/// Set density.
	void SetDensity(float density);
	/// Set friction.
	void SetFriction(float friction);
	/// Set restitution .
	void SetRestitution(float restitution);


	/// Return filter category bits
	int GetCategoryBits() const { return _fixtureDef.filter.categoryBits; }

	/// Return filter mask bits
	int GetMaskBits() const { return _fixtureDef.filter.maskBits; }

	/// Return filter group index
	int GetGroupIndex() const { return _fixtureDef.filter.groupIndex; }

	/// Return density
	float GetDensity() const { return _fixtureDef.density; }

	/// Return friction
	float GetFriction() const { return _fixtureDef.friction; }

	/// Return restitution
	float GetRestitution() const { return _fixtureDef.restitution; }

	/// Return mass
	float GetMass() const;
	/// Return inertia
	float GetInertia() const;
	/// Return mass center
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
	/// 
	RigidBody2D* _rigidBody;
	/// Cached world scale.
	Vector3 _cachedWorldScale;
};

}