#pragma once
#include "Component.h"
#include "AutoPhysics2D.h"

namespace Auto3D {

class Collider2D;
class PhysicsWorld2D;

/** 
* Rigid body type
*/
enum class BodyType2D
{
	Static = b2_staticBody,
	Kinematic = b2_kinematicBody,
	Dynamic = b2_dynamicBody
};

class RigidBody2D : public Component, public EnableSharedFromThis<RigidBody2D>
{
	REGISTER_OBJECT_ABSTRACT_CLASS(RigidBody2D, Component)
public:
	/**
	* @brief : Construct
	*/
	explicit RigidBody2D(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/** 
	* @brief : Override Start
	*/
	void Start() override;
	/**
	* @brief : Override Update
	*/
	void Update() override;
	/**
	* @brief : Create body
	*/
	void CreateBody();
	/**
	* @brief : Release body
	*/
	void ReleaseBody();
	/**
	* @brief : Set body type
	*/
	void SetBodyType(BodyType2D type);
	/**
	* @brief : Get box2D body
	*/
	b2Body* GetBody() { return _body; }
	/**
	* @brief : Return whether to calculate mass and inertia from collision shapes automatically
	*/
	bool GetUseFixtureMass() const { return _useFixtureMass; }
private:
	/// Physics world 2D
	SharedPtr<PhysicsWorld2D> _physicsWorld;
	/// box2D body define.
	b2BodyDef _bodyDef;
	/// box2D body.
	b2Body* _body;
	/// box2D mass data.
	b2MassData _massData;
	/// use fixture mass (calculate mass & inertia from collision shapes automatically.)
	bool _useFixtureMass;
};

}