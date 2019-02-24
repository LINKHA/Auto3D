#include "SpatialNode.h"

namespace Auto3D {


SpatialNode::SpatialNode(SharedPtr<Ambient> ambient)
	:Super(ambient),
	worldTransform(Matrix3x4F::IDENTITY),
	position(Vector3F::ZERO),
	rotation(QuaternionF::IDENTITY),
	scale(Vector3F::ONE)
{

}

SpatialNode::~SpatialNode()
{

}

}