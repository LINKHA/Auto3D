#include "Math/Ray.h"
#include "Scene/World.h"
#include "Engine/Components/Camera.h"
#include "Scene/Octree.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AOctreeNode)
	.constructor<>()
	.property("castShadows", &AOctreeNode::GetCastShadows, &AOctreeNode::SetCastShadows)
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

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

void AOctreeNode::SetCastShadows(bool enable)
{
    SetFlag(NF_CASTSHADOWS, enable);
}

void AOctreeNode::OnPrepareRender(unsigned frameNumber, ACamera* camera)
{
    _lastFrameNumber = frameNumber;
    _distance = camera->Distance(GetWorldPosition());
}

void AOctreeNode::OnRaycast(TVector<FRaycastResult>& dest, const FRay& ray, float maxDistance)
{
    float distance = ray.HitDistance(WorldBoundingBox());
    if (distance < maxDistance)
    {
        FRaycastResult res;
        res._position = ray._origin + distance * ray._direction;
        res._normal = -ray._direction;
        res._distance = distance;
        res._node = this;
        res._subObject = 0;
        dest.Push(res);
    }
}

void AOctreeNode::OnWorldSet(AWorld* newScene, AWorld*)
{
    /// Remove from current octree if any
    RemoveFromOctree();

    if (newScene)
    {
        // AOctree must be attached to the scene root as a child
        _octree = newScene->FindChild<AOctree>();
        // Transform may not be final yet. Schedule update but do not insert into octree yet
        if (_octree)
            _octree->QueueUpdate(this);
    }
}

void AOctreeNode::OnTransformChanged()
{
    ATransform::OnTransformChanged();
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