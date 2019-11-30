#include "Math/Ray.h"
#include "Scene/Scene.h"
#include "Engine/Components/Camera.h"
#include "Scene/Octree.h"

namespace Auto3D
{

AOctreeNode::AOctreeNode() :
    _octree(nullptr),
    _octant(nullptr),
    _lastFrameNumber(0),
    _distance(0.0f)
{
    SetFlag(NF_BOUNDING_BOX_DIRTY, true);
}

AOctreeNode::~AOctreeNode()
{
    RemoveFromOctree();
}

void AOctreeNode::RegisterObject()
{
    CopyBaseAttributes<AOctreeNode, ASpatialNode>();
    RegisterAttribute("castShadows", &AOctreeNode::CastShadows, &AOctreeNode::SetCastShadows, false);
}

void AOctreeNode::SetCastShadows(bool enable)
{
    SetFlag(NF_CASTSHADOWS, enable);
}

void AOctreeNode::OnPrepareRender(unsigned frameNumber, ACamera* camera)
{
    _lastFrameNumber = frameNumber;
    _distance = camera->Distance(GetWorldPosition());
}

void AOctreeNode::OnRaycast(TVector<RaycastResult>& dest, const Ray& ray, float maxDistance)
{
    float distance = ray.HitDistance(WorldBoundingBox());
    if (distance < maxDistance)
    {
        RaycastResult res;
        res._position = ray._origin + distance * ray._direction;
        res._normal = -ray._direction;
        res._distance = distance;
        res._node = this;
        res._subObject = 0;
        dest.Push(res);
    }
}

void AOctreeNode::OnSceneSet(Scene* newScene, Scene*)
{
    /// Remove from current octree if any
    RemoveFromOctree();

    if (newScene)
    {
        // Octree must be attached to the scene root as a child
        _octree = newScene->FindChild<Octree>();
        // Transform may not be final yet. Schedule update but do not insert into octree yet
        if (_octree)
            _octree->QueueUpdate(this);
    }
}

void AOctreeNode::OnTransformChanged()
{
    ASpatialNode::OnTransformChanged();
    SetFlag(NF_BOUNDING_BOX_DIRTY, true);

    if (!TestFlag(NF_OCTREE_UPDATE_QUEUED) && _octree)
        _octree->QueueUpdate(this);
}

void AOctreeNode::OnWorldBoundingBoxUpdate() const
{
    // The AOctreeNode base class does not have a defined _size, so represent as a point
    _worldBoundingBox.Define(GetWorldPosition());
    SetFlag(NF_BOUNDING_BOX_DIRTY, false);
}

void AOctreeNode::RemoveFromOctree()
{
    if (_octree)
    {
        _octree->RemoveNode(this);
        _octree = nullptr;
    }
}

}