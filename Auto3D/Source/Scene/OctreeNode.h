#pragma once

#include "Math/BoundingBox.h"
#include "Scene/SpatialNode.h"

namespace Auto3D
{

class ACamera;
class AOctree;
class FRay;
struct FOctant;
struct FRaycastResult;

/// Base class for scene nodes that insert themselves to the octree for rendering.
class AUTO_API AOctreeNode : public ASpatialNode
{
	DECLARE_CLASS(AOctreeNode, ASpatialNode)

    friend class AOctree;
public:
    /// Construct.
    AOctreeNode();
    /// Destruct. Remove self from the octree.
    ~AOctreeNode();

    /// Register attributes.
    static void RegisterObject();

    /// Prepare object for rendering. Reset framenumber and calculate distance from camera. Called by ARenderer.
    virtual void OnPrepareRender(unsigned frameNumber, ACamera* camera);
    /// Perform ray test on self and add possible hit to the result vector.
    virtual void OnRaycast(TVector<FRaycastResult>& dest, const FRay& ray, float maxDistance);

    /// Set whether to cast shadows. Default false on both lights and geometries.
    void SetCastShadows(bool enable);
    
    /// Return world space bounding box. Update if necessary.
    const TBoundingBoxF& WorldBoundingBox() const { if (TestFlag(NF_BOUNDING_BOX_DIRTY)) OnWorldBoundingBoxUpdate(); return _worldBoundingBox; }
    /// Return whether casts shadows.
    bool GetCastShadows() const { return TestFlag(NF_CASTSHADOWS); }
    /// Return current octree this node resides in.
    AOctree* GetOctree() const { return _octree; }
    /// Return current octree octant this node resides in.
    FOctant* GetOctant() const { return _octant; }
    /// Return distance from camera in the current view.
    float Distance() const { return _distance; }
    /// Return last frame number when was visible. The frames are counted by ARenderer internally and have no significance outside it.
    unsigned LastFrameNumber() const { return _lastFrameNumber; }

protected:
    /// Search for an octree from the scene root and add self to it.
    void OnSceneSet(AScene* newScene, AScene* oldScene) override;
    /// Handle the transform matrix changing.
    void OnTransformChanged() override;
    /// Recalculate the world space bounding box.
    virtual void OnWorldBoundingBoxUpdate() const;

    /// World space bounding box.
    mutable TBoundingBoxF _worldBoundingBox;
    /// Distance from camera in the current view.
    float _distance;
    /// Last frame number when was visible.
    unsigned _lastFrameNumber;

private:
    /// Remove from the current octree.
    void RemoveFromOctree();

    /// Current octree.
    AOctree* _octree;
    /// Current octree octant.
    FOctant* _octant;
};

}
