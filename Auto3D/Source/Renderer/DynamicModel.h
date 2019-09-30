#pragma once

#include "GeometryNode.h"

namespace Auto3D
{

class Model;
/// %Scene node that renders an unanimated model.
class AUTO_API DynamicModel : public GeometryNode
{
	REGISTER_OBJECT_CLASS(DynamicModel, GeometryNode)

public:
	/// Construct.
	DynamicModel();
	/// Destruct.
	~DynamicModel();

	/// Register factory and attributes.
	static void RegisterObject();


};

}
