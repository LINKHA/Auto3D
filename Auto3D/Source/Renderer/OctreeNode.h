#pragma once

#include "../Math/BoundingBox.h"
#include "../Scene/SpatialNode.h"

namespace Auto3D
{

class Camera;
class Octree;
class Ray;
struct Octant;
struct RaycastResult;

/// Base class for scene nodes that insert themselves to the octree for rendering.
class AUTO_API OctreeNode : public SpatialNode
{
    friend class Octree;

    REGISTER_OBJECT_CLASS(OctreeNode,SpatialNode)

public:
    /// Construct.
    OctreeNode();
    /// Destruct. Remove self from the octree.
    ~OctreeNode();

    /// Register attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and calculate distance from camera. Called by Renderer.
    virtual void OnPrepareRender(unsigned frameNumber, Camera* camera);
    /// Perform ray test on self and add possible hit to the result vector.
    virtual void OnRaycast(Vector<RaycastResult>& dest, const Ray& ray, float maxDistance);

    /// Set whether to cast shadows. Default false on both lights and geometries.
    void SetCastShadows(bool enable);
    
    /// Return world space bounding box. Update if necessary.
    const BoundingBox& WorldBoundingBox() const { if (TestFlag(NF_BOUNDING_BOX_DIRTY)) OnWorldBoundingBoxUpdate(); return _worldBoundingBox; }
    /// Return whether casts shadows.
    bool CastShadows() const { return TestFlag(NF_CASTSHADOWS); }
    /// Return current octree this node resides in.
    Octree* GetOctree() const { return _octree; }
    /// Return current octree octant this node resides in.
    Octant* GetOctant() const { return _octant; }
    /// Return distance from camera in the current view.
    float Distance() const { return _distance; }
    /// Return last frame number when was visible. The frames are counted by Renderer internally and have no significance outside it.
    unsigned LastFrameNumber() const { return _lastFrameNumber; }

protected:
    /// Search for an octree from the scene root and add self to it.
    void OnSceneSet(Scene* newScene, Scene* oldScene) override;
    /// Handle the transform matrix changing.
    void OnTransformChanged() override;
    /// Recalculate the world space bounding box.
    virtual void OnWorldBoundingBoxUpdate() const;

    /// World space bounding box.
    mutable BoundingBox _worldBoundingBox;
    /// Distance from camera in the current view.
    float _distance;
    /// Last frame number when was visible.
    unsigned _lastFrameNumber;

private:
    /// Remove from the current octree.
    void RemoveFromOctree();

    /// Current octree.
    Octree* _octree;
    /// Current octree octant.
    Octant* _octant;
};

}
