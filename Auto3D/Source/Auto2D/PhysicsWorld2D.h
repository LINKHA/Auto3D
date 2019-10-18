#pragma once
#include "Node2D.h"
#include "../Time/Time.h"

#include <Box2D.h>

namespace Auto3D {

class PhysicsWorld2D : public Node2D
{
	REGISTER_OBJECT_CLASS(PhysicsWorld2D, Node2D)
public:
	/// Construct
	PhysicsWorld2D();
	/// Destructor
	~PhysicsWorld2D();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Physics world update step simulation.
	void Update();
	/// Set fps
	void SetFPS(int fps);
	/// Return 2d dynamics world
	b2World* GetWorld() { return _world.Get(); }

private:

	unsigned int _fps;
	/// Time system.
	WeakPtr<Time> _time;
	/// Box2D world.
	UniquePtr<b2World> _world;
};


}