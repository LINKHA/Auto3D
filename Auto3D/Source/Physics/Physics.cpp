#include "Physics.h"

namespace Auto3D
{

Physics::Physics()
{
	RegisterSubsystem(this);
}

Physics::~Physics()
{
	RemoveSubsystem(this);
}

}