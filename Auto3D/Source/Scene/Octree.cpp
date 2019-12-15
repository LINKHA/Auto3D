#include "Debug/Log.h"
#include "Math/Ray.h"
#include "Octree.h"

#include <cassert>

#include "Debug/DebugNew.h"

namespace Auto3D
{

static const float DEFAULT_OCTREE_SIZE = 1000.0f;
static const int DEFAULT_OCTREE_LEVELS = 8;
static const int MAX_OCTREE_LEVELS = 256;

bool CompareRaycastResults(const FRaycastResult& lhs, const FRaycastResult& rhs)
{
    return lhs._distance < rhs._distance;
}

bool CompareNodeDistances(const TPair<AOctreeNode*, float>& lhs, const TPair<AOctreeNode*, float>& rhs)
{
    return lhs._second < rhs._second;
}

FOctant::FOctant() :
    _parent(nullptr),
    _numNodes(0)
{
    for (size_t i = 0; i < NUM_OCTANTS; ++i)
        _children[i] = nullptr;
}

void FOctant::Initialize(FOctant* parent, const TBoundingBoxF& boundingBox, int level)
{
    _worldBoundingBox = boundingBox;
    _center = _worldBoundingBox.Center();
    _halfSize = _worldBoundingBox.HalfSize();
    _cullingBox = TBoundingBoxF(_worldBoundingBox._min - _halfSize, _worldBoundingBox._max + _halfSize);
    _level = level;
    _parent = parent;
}

bool FOctant::FitBoundingBox(const TBoundingBoxF& box, const TVector3F& boxSize) const
{
    // If max split level, _size always OK, otherwise check that box is at least half _size of octant
    if (_level <= 1 || boxSize._x >= _halfSize._x || boxSize._y >= _halfSize._y || boxSize._z >= _halfSize._z)
        return true;
    // Also check if the box can not fit inside a child octant's culling box, in that case _size OK (must insert here)
    else
    {
        if (box._min._x <= _worldBoundingBox._min._x - 0.5f * _halfSize._x || box._min._y <= _worldBoundingBox._min._y - 0.5f * _halfSize._y ||
            box._min._z <= _worldBoundingBox._min._z - 0.5f * _halfSize._z || box._max._x >= _worldBoundingBox._max._x + 0.5f * _halfSize._x ||
            box._max._y >= _worldBoundingBox._max._y + 0.5f * _halfSize._y || box._max._z >= _worldBoundingBox._max._z + 0.5f * _halfSize._z)
            return true;
    }

    // Bounding box too small, should create a child octant
    return false;
}

REGISTER_CLASS
{
	using namespace rttr;
	registration::class_<AOctree>("AOctree")
	.constructor<>()
		.property("boundingBox", &AOctree::GetBoundingBoxAttr, &AOctree::SetBoundingBoxAttr)
		(
			metadata(SERIALIZABLE, "")
		)
		.property("numLevels", &AOctree::GetNumLevelsAttr, &AOctree::SetNumLevelsAttr)
		(
			metadata(SERIALIZABLE, "")
		)
	;
}

AOctree::AOctree()
{
    _root.Initialize(nullptr, TBoundingBoxF(-DEFAULT_OCTREE_SIZE, DEFAULT_OCTREE_SIZE), DEFAULT_OCTREE_LEVELS);
}

AOctree::~AOctree()
{
    DeleteChildOctants(&_root, true);
}

void AOctree::RegisterObject()
{
    RegisterFactory<AOctree>();
    CopyBaseAttributes<AOctree, ANode>();
    RegisterRefAttribute("boundingBox", &AOctree::GetBoundingBoxAttr, &AOctree::SetBoundingBoxAttr);
    RegisterAttribute("numLevels", &AOctree::GetNumLevelsAttr, &AOctree::SetNumLevelsAttr);
}

void AOctree::Update()
{
    PROFILE(UpdateOctree);

    for (auto it = _updateQueue.Begin(); it != _updateQueue.End(); ++it)
    {
        AOctreeNode* node = *it;
        // If node was removed before update could happen, a null pointer will be in its place
        if (node)
        {
            node->SetFlag(NF_OCTREE_UPDATE_QUEUED, false);

            // Do nothing if still fits the current octant
            const TBoundingBoxF& box = node->WorldBoundingBox();
            TVector3F boxSize = box.Size();
            FOctant* oldOctant = node->_octant;

		
            if (oldOctant && oldOctant->_cullingBox.IsInside(box) == EIntersection::INSIDE && oldOctant->FitBoundingBox(box, boxSize))
                continue;

            // Begin reinsert process. Start from root and check what level child needs to be used
            FOctant* newOctant = &_root;
            TVector3F boxCenter = box.Center();

            for (;;)
            {
                bool insertHere;
                // If node does not fit fully inside root octant, must remain in it
                if (newOctant == &_root)
                    insertHere = newOctant->_cullingBox.IsInside(box) != EIntersection::INSIDE || newOctant->FitBoundingBox(box, boxSize);
                else
                    insertHere = newOctant->FitBoundingBox(box, boxSize);

                if (insertHere)
                {
                    if (newOctant != oldOctant)
                    {
                        // Add first, then remove, because node count going to zero deletes the octree branch in question
                        AddNode(node, newOctant);
                        if (oldOctant)
                            RemoveNode(node, oldOctant);
                    }
                    break;
                }
                else
                    newOctant = CreateChildOctant(newOctant, newOctant->ChildIndex(boxCenter));
            }
        }
    }

    _updateQueue.Clear();
}

void AOctree::Resize(const TBoundingBoxF& boundingBox, int numLevels)
{
    PROFILE(ResizeOctree);

    // Collect nodes to the root and delete all child octants
    _updateQueue.Clear();
    CollectNodes(_updateQueue, &_root);
    DeleteChildOctants(&_root, false);
    _allocator.Reset();
    _root.Initialize(nullptr, boundingBox, Clamp(numLevels, 1, MAX_OCTREE_LEVELS));

    // Reinsert all nodes (recreates new child octants as necessary)
    Update();
}

void AOctree::RemoveNode(AOctreeNode* node)
{
    assert(node);
    RemoveNode(node, node->_octant);
    if (node->TestFlag(NF_OCTREE_UPDATE_QUEUED))
        CancelUpdate(node);
    node->_octant = nullptr;
}

void AOctree::QueueUpdate(AOctreeNode* node)
{
    assert(node);
    _updateQueue.Push(node);
    node->SetFlag(NF_OCTREE_UPDATE_QUEUED, true);
}

void AOctree::CancelUpdate(AOctreeNode* node)
{
    assert(node);
    auto it = _updateQueue.Find(node);
    if (it != _updateQueue.End())
        *it = nullptr;
    node->SetFlag(NF_OCTREE_UPDATE_QUEUED, false);
}

void AOctree::Raycast(TVector<FRaycastResult>& result, const FRay& ray, unsigned short nodeFlags, float maxDistance, unsigned layerMask)
{
    PROFILE(OctreeRaycast);

    result.Clear();
    CollectNodes(result, &_root, ray, nodeFlags, maxDistance, layerMask);
    Sort(result.Begin(), result.End(), CompareRaycastResults);
}

FRaycastResult AOctree::RaycastSingle(const FRay& ray, unsigned short nodeFlags, float maxDistance, unsigned layerMask)
{
    PROFILE(OctreeRaycastSingle);

    // Get first the potential hits
    _initialRes.Clear();
    CollectNodes(_initialRes, &_root, ray, nodeFlags, maxDistance, layerMask);
    Sort(_initialRes.Begin(), _initialRes.End(), CompareNodeDistances);

    // Then perform actual per-node ray tests and early-out when possible
    _finalRes.Clear();
    float closestHit = M_INFINITY;
    for (auto it = _initialRes.Begin(); it != _initialRes.End(); ++it)
    {
        if (it->_second < Min(closestHit, maxDistance))
        {
            size_t oldSize = _finalRes.Size();
            it->_first->OnRaycast(_finalRes, ray, maxDistance);
            if (_finalRes.Size() > oldSize)
                closestHit = Min(closestHit, _finalRes.Back()._distance);
        }
        else
            break;
    }

    if (_finalRes.Size())
    {
        Sort(_finalRes.Begin(), _finalRes.End(), CompareRaycastResults);
        return _finalRes.Front();
    }
    else
    {
        FRaycastResult emptyRes;
        emptyRes._position = emptyRes._normal = TVector3F::ZERO;
        emptyRes._distance = M_INFINITY;
        emptyRes._node = nullptr;
        emptyRes._subObject = 0;
        return emptyRes;
    }
}

void AOctree::SetBoundingBoxAttr(const TBoundingBoxF& boundingBox)
{
    _root._worldBoundingBox = boundingBox;
}

const TBoundingBoxF& AOctree::GetBoundingBoxAttr() const
{
    return _root._worldBoundingBox;
}

void AOctree::SetNumLevelsAttr(int numLevels)
{
    /// Setting the number of level (last attribute) triggers octree resize when deserializing
    Resize(_root._worldBoundingBox, numLevels);
}

int AOctree::GetNumLevelsAttr() const
{
    return _root._level;
}

void AOctree::AddNode(AOctreeNode* node, FOctant* octant)
{
    octant->_nodes.Push(node);
    node->_octant = octant;

    // Increment the node count in the whole parent branch
    while (octant)
    {
        ++octant->_numNodes;
        octant = octant->_parent;
    }
}

void AOctree::RemoveNode(AOctreeNode* node, FOctant* octant)
{
    // Do not set the node's octant pointer to zero, as the node may already be added into another octant
    octant->_nodes.Remove(node);
    
    // Decrement the node count in the whole parent branch and erase empty octants as necessary
    while (octant)
    {
        --octant->_numNodes;
        FOctant* next = octant->_parent;
        if (!octant->_numNodes && next)
            DeleteChildOctant(next, next->ChildIndex(octant->_center));
        octant = next;
    }
}

FOctant* AOctree::CreateChildOctant(FOctant* octant, size_t index)
{
    if (octant->_children[index])
        return octant->_children[index];

    TVector3F newMin = octant->_worldBoundingBox._min;
    TVector3F newMax = octant->_worldBoundingBox._max;
    const TVector3F& oldCenter = octant->_center;

    if (index & 1)
        newMin._x = oldCenter._x;
    else
        newMax._x = oldCenter._x;

    if (index & 2)
        newMin._y = oldCenter._y;
    else
        newMax._y = oldCenter._y;

    if (index & 4)
        newMin._z = oldCenter._z;
    else
        newMax._z = oldCenter._z;

    FOctant* child = _allocator.Allocate();
    child->Initialize(octant, TBoundingBoxF(newMin, newMax), octant->_level - 1);
    octant->_children[index] = child;

    return child;
}

void AOctree::DeleteChildOctant(FOctant* octant, size_t index)
{
    _allocator.Free(octant->_children[index]);
    octant->_children[index] = nullptr;
}

void AOctree::DeleteChildOctants(FOctant* octant, bool deletingOctree)
{
    for (auto it = octant->_nodes.Begin(); it != octant->_nodes.End(); ++it)
    {
        AOctreeNode* node = *it;
        node->_octant = nullptr;
        node->SetFlag(NF_OCTREE_UPDATE_QUEUED, false);
        if (deletingOctree)
            node->_octree = nullptr;
    }
    octant->_nodes.Clear();
    octant->_numNodes = 0;

    for (size_t i = 0; i < NUM_OCTANTS; ++i)
    {
        if (octant->_children[i])
        {
            DeleteChildOctants(octant->_children[i], deletingOctree);
            octant->_children[i] = nullptr;
        }
    }

    if (octant != &_root)
        _allocator.Free(octant);
}

void AOctree::CollectNodes(TVector<AOctreeNode*>& result, const FOctant* octant) const
{
    result.Push(octant->_nodes);

    for (size_t i = 0; i < NUM_OCTANTS; ++i)
    {
        if (octant->_children[i])
            CollectNodes(result, octant->_children[i]);
    }
}

void AOctree::CollectNodes(TVector<AOctreeNode*>& result, const FOctant* octant, unsigned short nodeFlags, unsigned layerMask) const
{
    const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
    for (auto it = octantNodes.Begin(); it != octantNodes.End(); ++it)
    {
        AOctreeNode* node = *it;
        if ((node->Flags() & nodeFlags) == nodeFlags && (node->GetLayerMask() & layerMask))
            result.Push(node);
    }

    for (size_t i = 0; i < NUM_OCTANTS; ++i)
    {
        if (octant->_children[i])
            CollectNodes(result, octant->_children[i], nodeFlags, layerMask);
    }
}

void AOctree::CollectNodes(TVector<FRaycastResult>& result, const FOctant* octant, const FRay& ray, unsigned short nodeFlags, 
    float maxDistance, unsigned layerMask) const
{
    float octantDist = ray.HitDistance(octant->_cullingBox);
    if (octantDist >= maxDistance)
        return;

    const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
    for (auto it = octantNodes.Begin(); it != octantNodes.End(); ++it)
    {
        AOctreeNode* node = *it;
        if ((node->Flags() & nodeFlags) == nodeFlags && (node->GetLayerMask() & layerMask))
            node->OnRaycast(result, ray, maxDistance);
    }

    for (size_t i = 0; i < NUM_OCTANTS; ++i)
    {
        if (octant->_children[i])
            CollectNodes(result, octant->_children[i], ray, nodeFlags, maxDistance, layerMask);
    }
}

void AOctree::CollectNodes(TVector<TPair<AOctreeNode*, float> >& result, const FOctant* octant, const FRay& ray, unsigned short nodeFlags,
    float maxDistance, unsigned layerMask) const
{
    float octantDist = ray.HitDistance(octant->_cullingBox);
    if (octantDist >= maxDistance)
        return;

    const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
    for (auto it = octantNodes.Begin(); it != octantNodes.End(); ++it)
    {
        AOctreeNode* node = *it;
        if ((node->Flags() & nodeFlags) == nodeFlags && (node->GetLayerMask() & layerMask))
        {
            float distance = ray.HitDistance(node->WorldBoundingBox());
            if (distance < maxDistance)
                result.Push(MakePair(node, distance));
        }
    }

    for (size_t i = 0; i < NUM_OCTANTS; ++i)
    {
        if (octant->_children[i])
            CollectNodes(result, octant->_children[i], ray, nodeFlags, maxDistance, layerMask);
    }
}

}
