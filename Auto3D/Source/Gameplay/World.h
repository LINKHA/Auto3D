#pragma once

#include "Gameplay/Actor.h"
#include "Adapter/Ptr.h"
namespace Auto3D
{

/// %AWorld root node, which also represents the whole world.
class AUTO_API AWorld : public AActor
{
	DECLARE_CLASS(AWorld, AActor)
public:
    /// Construct.
    AWorld();
    /// Destruct. The whole node tree is destroyed.
    ~AWorld();

	/// Overridable native event for when play begins for this actor. 
	virtual void BeginPlay();
	/// Function called every frame on this Actor.Override this function to implement custom logic to be executed every frame.
	virtual void Tick(float deltaSeconds);

	void OnRegister();
};


}

