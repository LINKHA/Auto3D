#pragma once

#include "Gameplay/Actor.h"

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
};


}

