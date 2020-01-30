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

	/// Add cctor to the world. This assigns a world-unique id to it. Called internally.
	void AddActor(AActor* actor);
	/// Remove actor from the world. This removes the id mapping but does not destroy the node. Called internally.
	void RemoveActor(AActor* actor);
	/// Add camera to the world. 
	void AddCamera(ACameraComponent* camera);
	/// Remove camera from the world.
	void RemoveCamera(ACameraComponent* camera);

	/// Save world as JSON text data to a binary stream. Return true on success.
	bool SaveJson(const FString& path);
	/// Load world from JSON data. Existing nodes will be destroyed. Return true on success.
	bool LoadJson(const FString& path);
	/// Destroy child nodes recursively, leaving the world empty.
	void Clear();


	THashMap<unsigned, AActor*> GetActors() { return _actors; }
	TVector<ACameraComponent*> GetCameras() { return _cameras; }
private:
	/// Map from id's to nodes.
	THashMap<unsigned, AActor*> _actors;
	/// ACamera to nodes
	TVector<ACameraComponent*> _cameras;
	/// Next free node id.
	unsigned _nextNodeId;
};


}

