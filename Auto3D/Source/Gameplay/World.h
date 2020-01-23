#pragma once

#include "Gameplay/Actor.h"
#include "Adapter/Ptr.h"
namespace Auto3D
{

class ACameraComponent;

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

	/// Add cctor to the world. This assigns a world-unique id to it. Called internally.
	void AddActor(SPtr<AActor> actor);
	/// Remove actor from the world. This removes the id mapping but does not destroy the node. Called internally.
	void RemoveActor(SPtr<AActor> actor);
	/// Add camera to the world. 
	void AddCamera(SPtr<ACameraComponent> camera);
	/// Remove camera from the world.
	void RemoveCamera(SPtr<ACameraComponent> camera);

	THashMap<unsigned, SPtr<AActor>> GetActors() { return _actors; }
private:
	/// Map from id's to nodes.
	THashMap<unsigned, SPtr<AActor>> _actors;
	/// ACamera to nodes
	TVector<SPtr<ACameraComponent>> _cameras;
	/// Next free node id.
	unsigned _nextNodeId;
};


}

