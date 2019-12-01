#pragma once

#include "Container/Allocator.h"
#include "Debug/Profiler.h"
#include "Math/BoundingBox.h"
#include "Scene/OctreeNode.h"

namespace Auto3D
{

static const size_t NUM_OCTANTS = 8;

class AOctree;
class AOctreeNode;
class FRay;

/// Structure for raycast query results.
struct AUTO_API FRaycastResult
{
    /// Hit world _position.
    TVector3F _position;
    /// Hit world normal.
    TVector3F _normal;
    /// Hit distance along the ray.
    float _distance;
    /// Hit node.
    AOctreeNode* _node;
    /// Hit geometry index or other, AGeometryNode subclass-specific subobject index.
    size_t _subObject;
};

/// %AOctree cell, contains up to 8 child octants.
struct AUTO_API FOctant
{
    /// Construct.
    FOctant();
   
    /// Initialize parent and bounds.
    void Initialize(FOctant* parent, const TBoundingBoxF& boundingBox, int level);
    /// Test if a node should be inserted in this octant or if a smaller child octant should be created.
    bool FitBoundingBox(const TBoundingBoxF& box, const TVector3F& boxSize) const;
    /// Return child octant index based on _position.
    size_t ChildIndex(const TVector3F& position) const { size_t ret = position._x < _center._x ? 0 : 1; ret += position._y < _center._y ? 0 : 2; ret += position._z < _center._z ? 0 : 4; return ret; }
    
    /// Expanded (loose) bounding box used for culling the octant and the nodes within it.
    TBoundingBoxF _cullingBox;
    /// Actual bounding box of the octant.
    TBoundingBoxF _worldBoundingBox;
    /// Bounding box center.
    TVector3F _center;
    /// Bounding box half _size.
    TVector3F _halfSize;
    /// Subdivision level.
    int _level;
    /// Nodes contained in the octant.
    TVector<AOctreeNode*> _nodes;
    /// Child octants.
    FOctant* _children[NUM_OCTANTS];
    /// Parent octant.
    FOctant* _parent;
    /// Number of nodes in this octant and the child octants combined.
    size_t _numNodes;
};

/// Acceleration structure for rendering. Should be created as a child of the scene root.
class AUTO_API AOctree : public ANode
{
    REGISTER_OBJECT(AOctree, ANode)

public:
    /// Construct.
    AOctree();
    /// Destruct. Delete all child octants and detach the nodes.
    ~AOctree();

    /// Register factory and attributes.
    static void RegisterObject();
    
    /// Process the queue of nodes to be reinserted.
    void Update();
    /// Resize octree.
    void Resize(const TBoundingBoxF& boundingBox, int numLevels);
    /// Remove a node from the octree.
    void RemoveNode(AOctreeNode* node);
    /// Queue a reinsertion for a node.
    void QueueUpdate(AOctreeNode* node);
    /// Cancel a pending reinsertion.
    void CancelUpdate(AOctreeNode* node);
    /// Query for nodes with a raycast and return all results.
    void Raycast(TVector<FRaycastResult>& result, const FRay& ray, unsigned short nodeFlags, float maxDistance = M_INFINITY, unsigned layerMask = LAYERMASK_ALL);
    /// Query for nodes with a raycast and return the closest result.
    FRaycastResult RaycastSingle(const FRay& ray, unsigned short nodeFlags, float maxDistance = M_INFINITY, unsigned layerMask = LAYERMASK_ALL);

    /// Query for nodes using a volume such as frustum or sphere.
    template <typename _Ty> void FindNodes(TVector<AOctreeNode*>& result, const _Ty& volume, unsigned short nodeFlags, unsigned layerMask = LAYERMASK_ALL) const
    {
        PROFILE(QueryOctree);
        CollectNodes(result, &_root, volume, nodeFlags, layerMask);
    }

    /// Query for nodes using a volume such as frustum or sphere. Invoke a function for each octant.
    template <typename _Ty> void FindNodes(const _Ty& volume, void(*callback)(TVector<AOctreeNode*>::ConstIterator, TVector<AOctreeNode*>::ConstIterator, bool)) const
    {
        PROFILE(QueryOctree);
        CollectNodesCallback(&_root, volume, callback);
    }

    /// Query for nodes using a volume such as frustum or sphere. Invoke a member function for each octant.
    template <typename _Ty, typename U> void FindNodes(const _Ty& volume, U* object, void (U::*callback)(TVector<AOctreeNode*>::ConstIterator, TVector<AOctreeNode*>::ConstIterator, bool)) const
    {
        PROFILE(QueryOctree);
        CollectNodesMemberCallback(&_root, volume, object, callback);
    }

private:
    /// Set bounding box. Used in serialization.
    void SetBoundingBoxAttr(const TBoundingBoxF& boundingBox);
    /// Return bounding box. Used in serialization.
    const TBoundingBoxF& BoundingBoxAttr() const;
    /// Set number of levels. Used in serialization.
    void SetNumLevelsAttr(int numLevels);
    /// Return number of levels. Used in serialization.
    int NumLevelsAttr() const;
    /// Add node to a specific octant.
    void AddNode(AOctreeNode* node, FOctant* octant);
    /// Remove node from an octant.
    void RemoveNode(AOctreeNode* node, FOctant* octant);
    /// Create a new child octant.
    FOctant* CreateChildOctant(FOctant* octant, size_t index);
    /// Delete one child octant.
    void DeleteChildOctant(FOctant* octant, size_t index);
    /// Delete a child octant hierarchy. If not deleting the octree for good, moves any nodes back to the root octant.
    void DeleteChildOctants(FOctant* octant, bool deletingOctree);
    /// Get all nodes from an octant recursively.
    void CollectNodes(TVector<AOctreeNode*>& result, const FOctant* octant) const;
    /// Get all visible nodes matching flags from an octant recursively.
    void CollectNodes(TVector<AOctreeNode*>& result, const FOctant* octant, unsigned short nodeFlags, unsigned layerMask) const;
    /// Get all visible nodes matching flags along a ray.
    void CollectNodes(TVector<FRaycastResult>& result, const FOctant* octant, const FRay& ray, unsigned short nodeFlags, float maxDistance, unsigned layerMask) const;
    /// Get all visible nodes matching flags that could be potential raycast hits.
    void CollectNodes(TVector<TPair<AOctreeNode*, float> >& result, const FOctant* octant, const FRay& ray, unsigned short nodeFlags, float maxDistance, unsigned layerMask) const;

    /// Collect nodes matching flags using a volume such as frustum or sphere.
    template <typename _Ty> void CollectNodes(TVector<AOctreeNode*>& result, const FOctant* octant, const _Ty& volume, unsigned short nodeFlags, unsigned layerMask) const
    {
		EIntersection::Type res = volume.IsInside(octant->_cullingBox);
        if (res == EIntersection::OUTSIDE)
            return;
        
        // If this octant is completely inside the volume, can include all contained octants and their nodes without further tests
        if (res == EIntersection::INSIDE)
            CollectNodes(result, octant, nodeFlags, layerMask);
        else
        {
            const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
            for (auto it = octantNodes.Begin(); it != octantNodes.End(); ++it)
            {
                AOctreeNode* node = *it;
                if ((node->Flags() & nodeFlags) == nodeFlags && (node->GetLayerMask() & layerMask) &&
                    volume.IsInsideFast(node->WorldBoundingBox()) != EIntersection::OUTSIDE)
                    result.Push(node);
            }
            
            for (size_t i = 0; i < NUM_OCTANTS; ++i)
            {
                if (octant->_children[i])
                    CollectNodes(result, octant->_children[i], volume, nodeFlags, layerMask);
            }
        }
    }
    
    /// Collect nodes from octant and child octants. Invoke a function for each octant.
    void CollectNodesCallback(const FOctant* octant, void(*callback)(TVector<AOctreeNode*>::ConstIterator, TVector<AOctreeNode*>::ConstIterator, bool)) const
    {
        const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
        callback(octantNodes.Begin(), octantNodes.End(), true);

        for (size_t i = 0; i < NUM_OCTANTS; ++i)
        {
            if (octant->_children[i])
                CollectNodesCallback(octant->_children[i], callback);
        }
    }

    /// Collect nodes using a volume such as frustum or sphere. Invoke a function for each octant.
    template <typename _Ty> void CollectNodesCallback(const FOctant* octant, const _Ty& volume, void(*callback)(TVector<AOctreeNode*>::ConstIterator, TVector<AOctreeNode*>::ConstIterator, bool)) const
    {
        Intersection res = volume.IsInside(octant->_cullingBox);
        if (res == OUTSIDE)
            return;

        // If this octant is completely inside the volume, can include all contained octants and their nodes without further tests
        if (res == INSIDE)
            CollectNodesCallback(octant, callback);
        else
        {
            const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
            callback(octantNodes.Begin(), octantNodes.End(), false);

            for (size_t i = 0; i < NUM_OCTANTS; ++i)
            {
                if (octant->_children[i])
                    CollectNodesCallback(octant->_children[i], volume, callback);
            }
        }
    }

    /// Collect nodes from octant and child octants. Invoke a member function for each octant.
    template <typename _Ty> void CollectNodesMemberCallback(const FOctant* octant, _Ty* object, void (_Ty::*callback)(TVector<AOctreeNode*>::ConstIterator, TVector<AOctreeNode*>::ConstIterator, bool)) const
    {
        const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
        (object->*callback)(octantNodes.Begin(), octantNodes.End(), true);

        for (size_t i = 0; i < NUM_OCTANTS; ++i)
        {
            if (octant->_children[i])
                CollectNodesMemberCallback(octant->_children[i], object, callback);
        }
    }

    /// Collect nodes using a volume such as frustum or sphere. Invoke a member function for each octant.
    template <typename _Ty, typename _Event> void CollectNodesMemberCallback(const FOctant* octant, const _Ty& volume, _Event* object, void (_Event::*callback)(TVector<AOctreeNode*>::ConstIterator, TVector<AOctreeNode*>::ConstIterator, bool)) const
    {
		EIntersection::Type res = volume.IsInside(octant->_cullingBox);
        if (res == EIntersection::OUTSIDE)
            return;

        // If this octant is completely inside the volume, can include all contained octants and their nodes without further tests
        if (res == EIntersection::INSIDE)
            CollectNodesMemberCallback(octant, object, callback);
        else
        {
            const TVector<AOctreeNode*>& octantNodes = octant->_nodes;
            (object->*callback)(octantNodes.Begin(), octantNodes.End(), false);

            for (size_t i = 0; i < NUM_OCTANTS; ++i)
            {
                if (octant->_children[i])
                    CollectNodesMemberCallback(octant->_children[i], volume, object, callback);
            }
        }
    }

    /// Queue of nodes to be reinserted.
    TVector<AOctreeNode*> _updateQueue;
    /// RaycastSingle initial coarse result.
    TVector<TPair<AOctreeNode*, float> > _initialRes;
    /// RaycastSingle final result.
    TVector<FRaycastResult> _finalRes;
    /// TAllocator for child octants.
    TAllocator<FOctant> _allocator;
    /// Root octant.
    FOctant _root;
};

}