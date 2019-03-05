#pragma once

#include "GeometryNode.h"

namespace Auto3D
{

class AUTO_API SkyBox : public GeometryNode
{
	REGISTER_OBJECT_CLASS(GeometryNode, OctreeNode)

public:
	/// Construct.
	SkyBox();
	/// Destruct.
	~SkyBox();

	/// Register factory and attributes.
	static void RegisterObject();

};

}