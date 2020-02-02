#pragma once

#include "Gameplay/Actor.h"
#include "Adapter/Ptr.h"
#include "IO/Stream.h"

namespace Auto3D
{

class ACameraComponent;

/// %AWorld root node, which also represents the whole world.
class AUTO_API AWorld : public AActor
{
	DECLARE_A_CLASS(AWorld, AActor)
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
	bool SaveJson(FStream& dest);
	/// Load world from JSON data. Existing nodes will be destroyed. Return true on success.
	bool LoadJson(const FString& path);
	/// Destroy child nodes recursively, leaving the world empty.
	void Clear();


	THashMap<unsigned, AActor*> GetActors() { return _actors; }
	TVector<ACameraComponent*> GetCameras() { return _cameras; }

	/// Define a layer name. There can be 32 different layers (indices 0-31.)
	void DefineLayer(unsigned char index, const FString& name);
	/// Define a tag name.
	void DefineTag(unsigned char index, const FString& name);

	/// Return the layer name-to-index map.
	const THashMap<FString, unsigned char>& Layers() const { return _defineLayers; }
	/// Return the tag name-to-index map.
	const THashMap<FString, unsigned char>& Tags() const { return _defineTags; }


public:
	/// Set layer names. Used in serialization.
	void SetLayerNamesAttr(FJSONValue names);
	/// Return layer names. Used in serialization.
	FJSONValue GetLayerNamesAttr() const;
	/// Set tag names. Used in serialization.
	void SetTagNamesAttr(FJSONValue names);
	/// Return tag names. Used in serialization.
	FJSONValue GetTagNamesAttr() const;

private:
	/// Map from id's to nodes.
	THashMap<unsigned, AActor*> _actors;
	/// ACamera to nodes
	TVector<ACameraComponent*> _cameras;
	/// Next free node id.
	unsigned _nextNodeId;

	/// Map from layer names to indices.
	THashMap<FString, unsigned char> _defineLayers;
	/// Map from tag names to indices.
	THashMap<FString, unsigned char> _defineTags;
};


}

