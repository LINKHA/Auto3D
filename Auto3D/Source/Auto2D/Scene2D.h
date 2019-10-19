#pragma once
#include "Node2D.h"

namespace Auto3D
{

class Camera2D;
class PhysicsWorld2D;

/// Location of all components of the Renderer2D
class AUTO_API Scene2D : public Node2D
{
	REGISTER_OBJECT_CLASS(Scene2D, Node2D)
public:
	/// The constructor
	Scene2D();
	/// The destructor
	~Scene2D();
	/// Register factory and attributes.
	static void RegisterObject();

	/// Save canvas to binary stream.
	void Save(Stream& dest) override;
	/// Load canvas from a binary stream. Existing Nodes will be destroyed. Return true on success.
	bool Load(Stream& source);
	/// Load canvas from JSON data. Existing Nodes will be destroyed. Return true on success.
	bool LoadJSON(const JSONValue& source);
	/// Load canvas from JSON text data read from a binary stream. Existing Nodes will be destroyed. Return true if the JSON was correctly parsed; otherwise the data may be partial.
	bool LoadJSON(Stream& source);
	/// Save canvas as JSON text data to a binary stream. Return true on success.
	bool SaveJSON(Stream& dest);
	/// Instantiate Node2D(s) from binary stream and return the root Node2D.
	Node2D* Instantiate(Stream& source);
	/// Instantiate Node2D(s) from JSON data and return the root Node2D.
	Node2D* InstantiateJSON(const JSONValue& source);
	/// Load JSON data as text from a binary stream, then instantiate Node2D(s) from it and return the root Node2D.
	Node2D* InstantiateJSON(Stream& source);
	/// Destroy child nodes recursively, leaving the canvas empty.
	void Clear();
	/// Find Node2D by id.
	Node2D* FindNode(unsigned id) const;
	/// Return all 2d node.
	const HashMap<unsigned, Node2D*>& GetAllNode() const;
	/// Return all camera vector
	Vector<Camera2D*>& GetAllCamera();
	/// Add node to the canvas. This assigns a canvas-unique id to it. Called internally.
	void AddNode(Node2D* node);
	/// Remove node from the canvas. This removes the id mapping but does not destroy the node. Called internally.
	void RemoveNode(Node2D* node);
	/// Add camera to the scene. 
	void AddCamera(Camera2D* camera) { _cameras.Push(camera); }
	/// Remove camera from the scene.
	void RemoveCamera(Camera2D* camera) { _cameras.Remove(camera); }
	/// Set 2D physics world.
	void SetPhysicsWorld(PhysicsWorld2D* physicsWorld);
	/// Get 2D physics world.
	PhysicsWorld2D* GetPhysicsWorld();

	using Node2D::Load;
	using Node2D::LoadJSON;
	using Node2D::SaveJSON;
private:
	/// Set layer names. Used in serialization.
	void SetLayerNamesAttr(JSONValue names);
	/// Return layer names. Used in serialization.
	JSONValue LayerNamesAttr() const;
	/// Set tag names. Used in serialization.
	void SetTagNamesAttr(JSONValue names);
	/// Return tag names. Used in serialization.
	JSONValue TagNamesAttr() const;

	/// Map from id to nodes.
	HashMap<unsigned, Node2D*> _nodes;
	/// Camera to nodes
	Vector<Camera2D*> _cameras;
	/// Next free node _id.
	unsigned _nextNodeId;

	/// Physics world 2D custom assign this variable
	PhysicsWorld2D* _physicsWorld;
};



}