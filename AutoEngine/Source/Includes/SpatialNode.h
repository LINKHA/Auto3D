#pragma once
#include "Node.h"

namespace Auto3D {

class SpatialNode : public Node
{
	REGISTER_OBJECT_CLASS(SpatialNode, Node)
public:
	/**
	* @brief : Construct
	*/
	explicit SpatialNode(SharedPtr<Ambient> ambient);

private:
	/// Update world transform matrix from spatial parent chain.
	void updateWorldTransform() const;

	/// World transform matrix.
	mutable Matrix3x4F worldTransform;
	/// Parent space position.
	Vector3F position;
	/// Parent space rotation.
	QuaternionF rotation;
	/// Parent space scale.
	Vector3F scale;
};

}