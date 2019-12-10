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
public:
	/// The constructor
	AScene2D();
	/// The destructor
	~AScene2D();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Save canvas to binary stream.
	void Save(FStream& dest) override;
	/// Load canvas from a binary stream. Existing Nodes will be destroyed. Return true on success.
	bool Load(FStream& source);
	/// Load canvas from JSON data. Existing Nodes will be destroyed. Return true on success.
	bool LoadJSON(const FJSONValue& source);
	/// Load canvas from JSON text data read from a binary stream. Existing Nodes will be destroyed. Return true if the JSON was correctly parsed; otherwise the data may be partial.
	bool LoadJSON(FStream& source);
	/// Save canvas as JSON text data to a binary stream. Return true on success.
	bool SaveJSON(FStream& dest);
	/// Instantiate ANode2D(s) from binary stream and return the root ANode2D.
	ANode2D* Instantiate(FStream& source);
	/// Instantiate ANode2D(s) from JSON data and return the root ANode2D.
	ANode2D* InstantiateJSON(const FJSONValue& source);
	/// Load JSON data as text from a binary stream, then instantiate ANode2D(s) from it and return the root ANode2D.
	ANode2D* InstantiateJSON(FStream& source);
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

	using ANode2D::Load;
	using ANode2D::LoadJSON;
	using ANode2D::SaveJSON;
private:
	/// Set layer names. Used in serialization.
	void SetLayerNamesAttr(FJSONValue names);
	/// Return layer names. Used in serialization.
	FJSONValue LayerNamesAttr() const;
	/// Set tag names. Used in serialization.
	void SetTagNamesAttr(FJSONValue names);
	/// Return tag names. Used in serialization.
	FJSONValue TagNamesAttr() const;

	/// Map from id to nodes.
	THashMap<unsigned, ANode2D*> _nodes;
	/// ACamera to nodes
	TVector<ACamera2D*> _cameras;
	/// Next free node _id.
	unsigned _nextNodeId;

	/// APhysics world 2D custom assign this variable
	APhysicsWorld2D* _physicsWorld;
};



}