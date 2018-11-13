#pragma once
#include "Component.h"
#include "AutoPhysics2D.h"

namespace Auto3D {

class PhysicsWorld2D : public Component//, public b2ContactListener, public b2Draw
{
	REGISTER_DERIVED_CLASS(PhysicsWorld2D, Component);
	DECLARE_OBJECT_SERIALIZE(PhysicsWorld2D);
public:
	explicit PhysicsWorld2D(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);
	/**
	* @brief : Return the Box2D physics world
	*/
	b2World* GetWorld() { return _world; }

	void Update()override;

private:
	b2World* _world;
	/// Is phtsics stepping
	bool _isPhysicsStepping{};
	/// Gravity default -9.81
	Vector2 _gravity;
	/// Velocity iterations default 8
	int _velocityIter{};
	/// Position iterations default 3
	int _positionIter{};

};

}