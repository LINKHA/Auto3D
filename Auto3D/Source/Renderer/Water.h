#pragma once
#include "DynamicModel.h"

namespace Auto3D
{

class AUTO_API Water : public DynamicModel
{
	REGISTER_OBJECT_CLASS(Water, DynamicModel)
public:
	/// Construct.
	Water();
	/// Destruct.
	virtual ~Water();

	/// Register factory and attributes.
	static void RegisterObject();


};

}