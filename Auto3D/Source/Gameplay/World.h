#pragma once

#include "Gameplay/Actor.h"
#include "Adapter/Ptr.h"
namespace Auto3D
{

/// %AWorld root node, which also represents the whole world.
class AUTO_API AWorld : public AActor
{
public:
    /// Construct.
    AWorld();
    /// Destruct. The whole node tree is destroyed.
    ~AWorld();

	void OnRegister();
};


}

