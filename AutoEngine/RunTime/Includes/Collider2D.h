#pragma once
#include "Component.h"
#include "AutoPhysics2D.h"

namespace Auto3D {
/// Collision shape type.
enum class ShapeType
{
	SHAPE_BOX = 0,
	SHAPE_SPHERE,
	SHAPE_STATICPLANE,
	SHAPE_CYLINDER,
	SHAPE_CAPSULE,
	SHAPE_CONE,
	SHAPE_TRIANGLEMESH,
	SHAPE_CONVEXHULL,
	SHAPE_TERRAIN,
	SHAPE_GIMPACTMESH
};

class Collider2D : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Collider2D, Component);
	DECLARE_OBJECT_SERIALIZE(Collider2D);
public:
	/**
	* @brief : Construct
	*/
	explicit Collider2D(Ambient* ambient);

	void CreateFixture();

	void ReleaseFixture();
	/**
	* @brief : Set trigger
	*/
	void SetTrigger(bool trigger)
	{

	}

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

private:
	/// Fixture def
	b2FixtureDef _fixtureDef;
	/// Box2D fixture
	b2Fixture* _fixture;

};

}