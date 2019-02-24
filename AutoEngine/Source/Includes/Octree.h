#pragma once
#include "OctreeNode.h"
#include "BoundingBox.h"

namespace Auto3D
{

static const size_t NUM_OCTANTS = 8;

class Octree;
class OctreeNode;
class Ray;

/// %Octree cell, contains up to 8 child octants.
struct Octant
{
	/// Construct.
	Octant();

	/// Initialize parent and bounds.
	void Initialize(SharedPtr<Octant> parent, const BoundingBox& boundingBox, int level);
	/// Test if a node should be inserted in this octant or if a smaller child octant should be created.
	bool FitBoundingBox(const BoundingBox& box, const Vector3F& boxSize) const;
	/// Return child octant index based on position.
	size_t ChildIndex(const Vector3F& position) const { size_t ret = position._x < center._x ? 0 : 1; ret += position._y < center._y ? 0 : 2; ret += position._z < center._z ? 0 : 4; return ret; }

	/// Expanded (loose) bounding box used for culling the octant and the nodes within it.
	BoundingBox cullingBox;
	/// Actual bounding box of the octant.
	BoundingBox worldBoundingBox;
	/// Bounding box center.
	Vector3F center;
	/// Bounding box half size.
	Vector3F halfSize;
	/// Subdivision level.
	int level;
	/// Nodes contained in the octant.
	VECTOR<OctreeNode*> nodes;
	/// Child octants.
	SharedPtr<Octant> children[NUM_OCTANTS];
	/// Parent octant.
	SharedPtr<Octant> parent;
	/// Number of nodes in this octant and the child octants combined.
	size_t numNodes;
};


/// Acceleration structure for rendering. Should be created as a child of the scene root.
class Octree : public Node
{
	REGISTER_OBJECT_CLASS(Octree,Node)
public:
	/**
	* @brief : Construct
	*/
	explicit Octree(SharedPtr<Ambient> ambient);
};
}