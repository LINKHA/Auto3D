#include "Math/Ray.h"
#include "Scene/Scene.h"
#include "Engine/Components/Camera.h"
#include "Scene/Octree.h"

namespace Auto3D
{

OctreeNode::OctreeNode() :
    _octree(nullptr),
    _octant(nullptr),
    _lastFrameNumber(0),
    _distance(0.0f)
{
    SetFlag(NF_BOUNDING_BOX_DIRTY, true);
}

OctreeNode::~OctreeNode()
{
    RemoveFromOctree();
}

void OctreeNode::RegisterObject()
{
    CopyBaseAttributes<OctreeNode, ASpatialNode>();
    RegisterAttribute("castShadows", &OctreeNode::CastShadows, &OctreeNode::SetCastShadows, false);
}

void OctreeNode::SetCastShadows(bool enable)
{
    SetFlag(NF_CASTSHADOWS, enable);
}

void OctreeNode::OnPrepareRender(unsigned frameNumber, Camera* camera)
{
    _lastFrameNumber = frameNumber;
    _distance = camera->Distance(GetWorldPosition());
}

void OctreeNode::OnRaycast(Vector<RaycastResult>& dest, const Ray& ray, float maxDistance)
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

void OctreeNode::OnSceneSet(Scene* newScene, Scene*)
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

void OctreeNode::OnTransformChanged()
{
    ASpatialNode::OnTransformChanged();
    SetFlag(NF_BOUNDING_BOX_DIRTY, true);

    if (!TestFlag(NF_OCTREE_UPDATE_QUEUED) && _octree)
        _octree->QueueUpdate(this);
}

void OctreeNode::OnWorldBoundingBoxUpdate() const
{
    // The OctreeNode base class does not have a defined _size, so represent as a point
    _worldBoundingBox.Define(GetWorldPosition());
    SetFlag(NF_BOUNDING_BOX_DIRTY, false);
}

void OctreeNode::RemoveFromOctree()
{
    if (_octree)
    {
        _octree->RemoveNode(this);
        _octree = nullptr;
    }
}

}