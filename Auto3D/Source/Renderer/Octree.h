#pragma once

#include "Container/Allocator.h"
#include "Debug/Profiler.h"
#include "Math/BoundingBox.h"
#include "OctreeNode.h"

namespace Auto3D
{

static const size_t NUM_OCTANTS = 8;

class Octree;
class OctreeNode;
class Ray;

/// Structure for raycast query results.
struct AUTO_API RaycastResult
{
    /// Hit world _position.
    Vector3F _position;
    /// Hit world normal.
    Vector3F _normal;
    /// Hit distance along the ray.
    float _distance;
    /// Hit node.
    OctreeNode* _node;
    /// Hit geometry index or other, GeometryNode subclass-specific subobject index.
    size_t _subObject;
};

/// %Octree cell, contains up to 8 child octants.
struct AUTO_API Octant
{
    /// Construct.
    Octant();
   
    /// Initialize parent and bounds.
    void Initialize(Octant* parent, const BoundingBoxF& boundingBox, int level);
    /// Test if a node should be inserted in this octant or if a smaller child octant should be created.
    bool FitBoundingBox(const BoundingBoxF& box, const Vector3F& boxSize) const;
    /// Return child octant index based on _position.
    size_t ChildIndex(const Vector3F& position) const { size_t ret = position._x < _center._x ? 0 : 1; ret += position._y < _center._y ? 0 : 2; ret += position._z < _center._z ? 0 : 4; return ret; }
    
    /// Expanded (loose) bounding box used for culling the octant and the nodes within it.
    BoundingBoxF _cullingBox;
    /// Actual bounding box of the octant.
    BoundingBoxF _worldBoundingBox;
    /// Bounding box center.
    Vector3F _center;
    /// Bounding box half _size.
    Vector3F _halfSize;
    /// Subdivision level.
    int _level;
    /// Nodes contained in the octant.
    Vector<OctreeNode*> _nodes;
    /// Child octants.
    Octant* _children[NUM_OCTANTS];
    /// Parent octant.
    Octant* _parent;
    /// Number of nodes in this octant and the child octants combined.
    size_t _numNodes;
};

/// Acceleration structure for rendering. Should be created as a child of the scene root.
class AUTO_API Octree : public Node
{
    REGISTER_OBJECT_CLASS(Octree, Node)

public:
    /// Construct.
    Octree();
    /// Destruct. Delete all child octants and detach the nodes.
    ~Octree();

    /// Register factory and attributes.
    static void RegisterObject();
    
    /// Process the queue of nodes to be reinserted.
    void Update();
    /// Resize octree.
    void Resize(const BoundingBoxF& boundingBox, int numLevels);
    /// Remove a node from the octree.
    void RemoveNode(OctreeNode* node);
    /// Queue a reinsertion for a node.
    void QueueUpdate(OctreeNode* node);
    /// Cancel a pending reinsertion.
    void CancelUpdate(OctreeNode* node);
    /// Query for nodes with a raycast and return all results.
    void Raycast(Vector<RaycastResult>& result, const Ray& ray, unsigned short nodeFlags, float maxDistance = M_INFINITY, unsigned layerMask = LAYERMASK_ALL);
    /// Query for nodes with a raycast and return the closest result.
    RaycastResult RaycastSingle(const Ray& ray, unsigned short nodeFlags, float maxDistance = M_INFINITY, unsigned layerMask = LAYERMASK_ALL);

    /// Query for nodes using a volume such as frustum or sphere.
    template <typename _Ty> void FindNodes(Vector<OctreeNode*>& result, const _Ty& volume, unsigned short nodeFlags, unsigned layerMask = LAYERMASK_ALL) const
    {
        PROFILE(QueryOctree);
        CollectNodes(result, &_root, volume, nodeFlags, layerMask);
    }

    /// Query for nodes using a volume such as frustum or sphere. Invoke a function for each octant.
    template <typename _Ty> void FindNodes(const _Ty& volume, void(*callback)(Vector<OctreeNode*>::ConstIterator, Vector<OctreeNode*>::ConstIterator, bool)) const
    {
        PROFILE(QueryOctree);
        CollectNodesCallback(&_root, volume, callback);
    }

    /// Query for nodes using a volume such as frustum or sphere. Invoke a member function for each octant.
    template <typename _Ty, typename U> void FindNodes(const _Ty& volume, U* object, void (U::*callback)(Vector<OctreeNode*>::ConstIterator, Vector<OctreeNode*>::ConstIterator, bool)) const
    {
        PROFILE(QueryOctree);
        CollectNodesMemberCallback(&_root, volume, object, callback);
    }

private:
    /// Set bounding box. Used in serialization.
    void SetBoundingBoxAttr(const BoundingBoxF& boundingBox);
    /// Return bounding box. Used in serialization.
    const BoundingBoxF& BoundingBoxAttr() const;
    /// Set number of levels. Used in serialization.
    void SetNumLevelsAttr(int numLevels);
    /// Return number of levels. Used in serialization.
    int NumLevelsAttr() const;
    /// Add node to a specific octant.
    void AddNode(OctreeNode* node, Octant* octant);
    /// Remove node from an octant.
    void RemoveNode(OctreeNode* node, Octant* octant);
    /// Create a new child octant.
    Octant* CreateChildOctant(Octant* octant, size_t index);
    /// Delete one child octant.
    void DeleteChildOctant(Octant* octant, size_t index);
    /// Delete a child octant hierarchy. If not deleting the octree for good, moves any nodes back to the root octant.
    void DeleteChildOctants(Octant* octant, bool deletingOctree);
    /// Get all nodes from an octant recursively.
    void CollectNodes(Vector<OctreeNode*>& result, const Octant* octant) const;
    /// Get all visible nodes matching flags from an octant recursively.
    void CollectNodes(Vector<OctreeNode*>& result, const Octant* octant, unsigned short nodeFlags, unsigned layerMask) const;
    /// Get all visible nodes matching flags along a ray.
    void CollectNodes(Vector<RaycastResult>& result, const Octant* octant, const Ray& ray, unsigned short nodeFlags, float maxDistance, unsigned layerMask) const;
    /// Get all visible nodes matching flags that could be potential raycast hits.
    void CollectNodes(Vector<Pair<OctreeNode*, float> >& result, const Octant* octant, const Ray& ray, unsigned short nodeFlags, float maxDistance, unsigned layerMask) const;

    /// Collect nodes matching flags using a volume such as frustum or sphere.
    template <typename _Ty> void CollectNodes(Vector<OctreeNode*>& result, const Octant* octant, const _Ty& volume, unsigned short nodeFlags, unsigned layerMask) const
    {
        Intersection res = volume.IsInside(octant->_cullingBox);
        if (res == OUTSIDE)
            return;
        
        // If this octant is completely inside the volume, can include all contained octants and their nodes without further tests
        if (res == INSIDE)
            CollectNodes(result, octant, nodeFlags, layerMask);
        else
        {
            const Vector<OctreeNode*>& octantNodes = octant->_nodes;
            for (auto it = octantNodes.Begin(); it != octantNodes.End(); ++it)
            {
                OctreeNode* node = *it;
                if ((node->Flags() & nodeFlags) == nodeFlags && (node->GetLayerMask() & layerMask) &&
                    volume.IsInsideFast(node->WorldBoundingBox()) != OUTSIDE)
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
    void CollectNodesCallback(const Octant* octant, void(*callback)(Vector<OctreeNode*>::ConstIterator, Vector<OctreeNode*>::ConstIterator, bool)) const
    {
        const Vector<OctreeNode*>& octantNodes = octant->_nodes;
        callback(octantNodes.Begin(), octantNodes.End(), true);

        for (size_t i = 0; i < NUM_OCTANTS; ++i)
        {
            if (octant->_children[i])
                CollectNodesCallback(octant->_children[i], callback);
        }
    }

    /// Collect nodes using a volume such as frustum or sphere. Invoke a function for each octant.
    template <typename _Ty> void CollectNodesCallback(const Octant* octant, const _Ty& volume, void(*callback)(Vector<OctreeNode*>::ConstIterator, Vector<OctreeNode*>::ConstIterator, bool)) const
    {
        Intersection res = volume.IsInside(octant->_cullingBox);
        if (res == OUTSIDE)
            return;

        // If this octant is completely inside the volume, can include all contained octants and their nodes without further tests
        if (res == INSIDE)
            CollectNodesCallback(octant, callback);
        else
        {
            const Vector<OctreeNode*>& octantNodes = octant->_nodes;
            callback(octantNodes.Begin(), octantNodes.End(), false);

            for (size_t i = 0; i < NUM_OCTANTS; ++i)
            {
                if (octant->_children[i])
                    CollectNodesCallback(octant->_children[i], volume, callback);
            }
        }
    }

    /// Collect nodes from octant and child octants. Invoke a member function for each octant.
    template <typename _Ty> void CollectNodesMemberCallback(const Octant* octant, _Ty* object, void (_Ty::*callback)(Vector<OctreeNode*>::ConstIterator, Vector<OctreeNode*>::ConstIterator, bool)) const
    {
        const Vector<OctreeNode*>& octantNodes = octant->_nodes;
        (object->*callback)(octantNodes.Begin(), octantNodes.End(), true);

        for (size_t i = 0; i < NUM_OCTANTS; ++i)
        {
            if (octant->_children[i])
                CollectNodesMemberCallback(octant->_children[i], object, callback);
        }
    }

    /// Collect nodes using a volume such as frustum or sphere. Invoke a member function for each octant.
    template <typename _Ty, typename _Event> void CollectNodesMemberCallback(const Octant* octant, const _Ty& volume, _Event* object, void (_Event::*callback)(Vector<OctreeNode*>::ConstIterator, Vector<OctreeNode*>::ConstIterator, bool)) const
    {
        Intersection res = volume.IsInside(octant->_cullingBox);
        if (res == OUTSIDE)
            return;

        // If this octant is completely inside the volume, can include all contained octants and their nodes without further tests
        if (res == INSIDE)
            CollectNodesMemberCallback(octant, object, callback);
        else
        {
            const Vector<OctreeNode*>& octantNodes = octant->_nodes;
            (object->*callback)(octantNodes.Begin(), octantNodes.End(), false);

            for (size_t i = 0; i < NUM_OCTANTS; ++i)
            {
                if (octant->_children[i])
                    CollectNodesMemberCallback(octant->_children[i], volume, object, callback);
            }
        }
    }

    /// Queue of nodes to be reinserted.
    Vector<OctreeNode*> _updateQueue;
    /// RaycastSingle initial coarse result.
    Vector<Pair<OctreeNode*, float> > _initialRes;
    /// RaycastSingle final result.
    Vector<RaycastResult> _finalRes;
    /// Allocator for child octants.
    Allocator<Octant> _allocator;
    /// Root octant.
    Octant _root;
};

}