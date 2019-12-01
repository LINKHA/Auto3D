#pragma once

#include "Node.h"

namespace Auto3D
{

class ACamera;
class APhysicsWorld;
class ASkyBox;

/// %AScene root node, which also represents the whole scene.
class AUTO_API AScene : public ANode
{
    REGISTER_OBJECT(AScene, ANode)

public:
    /// Construct.
    AScene();
    /// Destruct. The whole node tree is destroyed.
    ~AScene();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Save scene to binary stream.
    void Save(FStream& dest) override;
    /// Load scene from a binary stream. Existing nodes will be destroyed. Return true on success.
    bool Load(FStream& source);
    /// Load scene from JSON data. Existing nodes will be destroyed. Return true on success.
    bool LoadJSON(const FJSONValue& source);
    /// Load scene from JSON text data read from a binary stream. Existing nodes will be destroyed. Return true if the JSON was correctly parsed; otherwise the data may be partial.
    bool LoadJSON(FStream& source);
    /// Save scene as JSON text data to a binary stream. Return true on success.
    bool SaveJSON(FStream& dest);
    /// Instantiate node(s) from binary stream and return the root node.
    ANode* Instantiate(FStream& source);
    /// Instantiate node(s) from JSON data and return the root node.
    ANode* InstantiateJSON(const FJSONValue& source);
    /// Load JSON data as text from a binary stream, then instantiate node(s) from it and return the root node.
    ANode* InstantiateJSON(FStream& source);
    /// Destroy child nodes recursively, leaving the scene empty.
    void Clear();
    /// Find node by _id.
    ANode* FindNode(unsigned id) const;
	/// Return all camera vector
	TVector<ACamera*>& GetAllCamera();
    /// Add node to the scene. This assigns a scene-unique id to it. Called internally.
    void AddNode(ANode* node);
    /// Remove node from the scene. This removes the id mapping but does not destroy the node. Called internally.
    void RemoveNode(ANode* node);
	/// Add camera to the scene. 
	void AddCamera(ACamera* camera) { _cameras.Push(camera); }
	/// Remove camera from the scene.
	void RemoveCamera(ACamera* camera) { _cameras.Remove(camera); }
	/// Set 3D physics world.
	void SetPhysicsWorld(APhysicsWorld* physicsWorld);
	/// Set skybox.
	void SetSkyBox(ASkyBox* skybox);
	/// Get 3D physics world.
	APhysicsWorld* GetPhysicsWorld();
	/// Get skybox.
	ASkyBox* GetSkyBox();
	/// Setup shadow maps.
	void SetupShadowMap(size_t num, int size);



    using ANode::Load;
    using ANode::LoadJSON;
    using ANode::SaveJSON;

private:
    /// Set layer names. Used in serialization.
    void SetLayerNamesAttr(FJSONValue names);
    /// Return layer names. Used in serialization.
    FJSONValue LayerNamesAttr() const;
    /// Set tag names. Used in serialization.
    void SetTagNamesAttr(FJSONValue names);
    /// Return tag names. Used in serialization.
    FJSONValue TagNamesAttr() const;

    /// Map from id's to nodes.
    THashMap<unsigned, ANode*> _nodes;
	/// ACamera to nodes
	TVector<ACamera*> _cameras;
    /// Next free node id.
    unsigned _nextNodeId;

	/// APhysics world custom assign this variable
	APhysicsWorld* _physicsWorld;
	/// Skybox in this scene.
	ASkyBox* _skybox;
};

/// Register AScene related object factories and attributes.
AUTO_API void RegisterSceneLibrary();

}

