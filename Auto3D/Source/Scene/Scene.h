#pragma once

#include "Node.h"

namespace Auto3D
{

class ACamera;
class PhysicsWorld;
class ASkyBox;

/// %Scene root node, which also represents the whole scene.
class AUTO_API Scene : public Node
{
    REGISTER_OBJECT_CLASS(Scene, Node)

public:
    /// Construct.
    Scene();
    /// Destruct. The whole node tree is destroyed.
    ~Scene();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Save scene to binary stream.
    void Save(Stream& dest) override;
    /// Load scene from a binary stream. Existing nodes will be destroyed. Return true on success.
    bool Load(Stream& source);
    /// Load scene from JSON data. Existing nodes will be destroyed. Return true on success.
    bool LoadJSON(const JSONValue& source);
    /// Load scene from JSON text data read from a binary stream. Existing nodes will be destroyed. Return true if the JSON was correctly parsed; otherwise the data may be partial.
    bool LoadJSON(Stream& source);
    /// Save scene as JSON text data to a binary stream. Return true on success.
    bool SaveJSON(Stream& dest);
    /// Instantiate node(s) from binary stream and return the root node.
    Node* Instantiate(Stream& source);
    /// Instantiate node(s) from JSON data and return the root node.
    Node* InstantiateJSON(const JSONValue& source);
    /// Load JSON data as text from a binary stream, then instantiate node(s) from it and return the root node.
    Node* InstantiateJSON(Stream& source);
    /// Destroy child nodes recursively, leaving the scene empty.
    void Clear();
    /// Find node by _id.
    Node* FindNode(unsigned id) const;
	/// Return all camera vector
	TVector<ACamera*>& GetAllCamera();
    /// Add node to the scene. This assigns a scene-unique id to it. Called internally.
    void AddNode(Node* node);
    /// Remove node from the scene. This removes the id mapping but does not destroy the node. Called internally.
    void RemoveNode(Node* node);
	/// Add camera to the scene. 
	void AddCamera(ACamera* camera) { _cameras.Push(camera); }
	/// Remove camera from the scene.
	void RemoveCamera(ACamera* camera) { _cameras.Remove(camera); }
	/// Set 3D physics world.
	void SetPhysicsWorld(PhysicsWorld* physicsWorld);
	/// Set skybox.
	void SetSkyBox(ASkyBox* skybox);
	/// Get 3D physics world.
	PhysicsWorld* GetPhysicsWorld();
	/// Get skybox.
	ASkyBox* GetSkyBox();
	/// Setup shadow maps.
	void SetupShadowMap(size_t num, int size);



    using Node::Load;
    using Node::LoadJSON;
    using Node::SaveJSON;

private:
    /// Set layer names. Used in serialization.
    void SetLayerNamesAttr(JSONValue names);
    /// Return layer names. Used in serialization.
    JSONValue LayerNamesAttr() const;
    /// Set tag names. Used in serialization.
    void SetTagNamesAttr(JSONValue names);
    /// Return tag names. Used in serialization.
    JSONValue TagNamesAttr() const;

    /// Map from id's to nodes.
    THashMap<unsigned, Node*> _nodes;
	/// ACamera to nodes
	TVector<ACamera*> _cameras;
    /// Next free node id.
    unsigned _nextNodeId;

	/// Physics world custom assign this variable
	PhysicsWorld* _physicsWorld;
	/// Skybox in this scene.
	ASkyBox* _skybox;
};

/// Register Scene related object factories and attributes.
AUTO_API void RegisterSceneLibrary();

}

