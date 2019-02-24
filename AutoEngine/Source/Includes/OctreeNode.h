#pragma once
#include "Node.h"

namespace Auto3D {
	class Octree;
	struct Octant;
class OctreeNode : public Node
{
	REGISTER_OBJECT_CLASS(OctreeNode, Node)
public:
	/**
	* @brief : Construct
	*/
	explicit OctreeNode(SharedPtr<Ambient> ambient);

private:
	/// Remove from the current octree.
	void RemoveFromOctree();

	/// Current octree.
	SharedPtr<Octree> octree;
	/// Current octree octant.
	SharedPtr<Octant> octant;
};

}