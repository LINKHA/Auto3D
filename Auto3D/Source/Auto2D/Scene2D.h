#pragma once
#include "Node2D.h"

namespace Auto3D
{

class ACamera2D;
class APhysicsWorld2D;

/// Location of all components of the Renderer2D
class AUTO_API AScene2D : public ANode2D
{
	DECLARE_CLASS(AScene2D, ANode2D)

	DECLARE_CLASS_NEW(AScene2D, ANode2D)
public:
	/// The constructor
	AScene2D();
	/// The destructor
	~AScene2D();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Save canvas to binary stream.
	bool Save(FStream& dest);
	/// Load canvas from a binary stream. Existing Nodes will be destroyed. Return true on success.
	bool Load(FStream& source);
	/// Load canvas from JSON data. Existing Nodes will be destroyed. Return true on success.
	bool LoadJSON(const FJSONValue& source);
	/// Load canvas from JSON text data read from a binary stream. Existing Nodes will be destroyed. Return true if the JSON was correctly parsed; otherwise the data may be partial.
	bool LoadJSON(FStream& source);
	/// Save canvas as JSON text data to a binary stream. Return true on success.
	bool SaveJSON(FStream& dest);
	/// Destroy child nodes recursively, leaving the canvas empty.
	void Clear();
	/// Find ANode2D by id.
	ANode2D* FindNode(unsigned id) const;
	/// Return all 2d node.
	const THashMap<unsigned, ANode2D*>& GetAllNode() const;
	/// Return all camera vector
	TVector<ACamera2D*>& GetCameras();
	/// Add node to the canvas. This assigns a canvas-unique id to it. Called internally.
	void AddNode(ANode2D* node);
	/// Remove node from the canvas. This removes the id mapping but does not destroy the node. Called internally.
	void RemoveNode(ANode2D* node);
	/// Add camera to the scene. 
	void AddCamera(ACamera2D* camera) { _cameras.Push(camera); }
	/// Remove camera from the scene.
	void RemoveCamera(ACamera2D* camera) { _cameras.Remove(camera); }
	/// Set 2D physics world.
	void SetPhysicsWorld(APhysicsWorld2D* physicsWorld);
	/// Get 2D physics world.
	APhysicsWorld2D* GetPhysicsWorld();

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
	

	/// Map from id to nodes.
	THashMap<unsigned, ANode2D*> _nodes;
	/// ACamera to nodes
	TVector<ACamera2D*> _cameras;
	/// Next free node _id.
	unsigned _nextNodeId;

	/// APhysics world 2D custom assign this variable
	APhysicsWorld2D* _physicsWorld;

	/// Map from layer names to indices.
	THashMap<FString, unsigned char> _defineLayers;
	/// Map from tag names to indices.
	THashMap<FString, unsigned char> _defineTags;
};



}