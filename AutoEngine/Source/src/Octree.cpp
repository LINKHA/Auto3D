#include "Octree.h"


namespace Auto3D
{
Octant::Octant() :
	parent(nullptr),
	numNodes(0)
{
	for (size_t i = 0; i < NUM_OCTANTS; ++i)
		children[i] = nullptr;
}

void Octant::Initialize(SharedPtr<Octant> parent_, const BoundingBox& boundingBox, int level_)
{
	worldBoundingBox = boundingBox;
	center = worldBoundingBox.Center();
	halfSize = worldBoundingBox.HalfSize();
	cullingBox = BoundingBox(worldBoundingBox.min - halfSize, worldBoundingBox.max + halfSize);
	level = level_;
	parent = parent_;
}

bool Octant::FitBoundingBox(const BoundingBox& box, const Vector3F& boxSize) const
{
	// If max split level, size always OK, otherwise check that box is at least half size of octant
	if (level <= 1 || boxSize._x >= halfSize._x || boxSize._y >= halfSize._y || boxSize._z >= halfSize._z)
		return true;
	// Also check if the box can not fit inside a child octant's culling box, in that case size OK (must insert here)
	else
	{
		if (box.min._x <= worldBoundingBox.min._x - 0.5f * halfSize._x || box.min._y <= worldBoundingBox.min._y - 0.5f * halfSize._y ||
			box.min._z <= worldBoundingBox.min._z - 0.5f * halfSize._z || box.max._x >= worldBoundingBox.max._x + 0.5f * halfSize._x ||
			box.max._y >= worldBoundingBox.max._y + 0.5f * halfSize._y || box.max._z >= worldBoundingBox.max._z + 0.5f * halfSize._z)
			return true;
	}

	// Bounding box too small, should create a child octant
	return false;
}

Octree::Octree(SharedPtr<Ambient> ambient)
	:Super(ambient)
{

}

Octree::~Octree()
{

}

}