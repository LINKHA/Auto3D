#pragma once
#include "DynamicModel.h"

namespace Auto3D
{

class AUTO_API AWater : public ADynamicModel
{
	REGISTER_OBJECT_CLASS(AWater, ADynamicModel)
public:
	/// Construct.
	AWater();
	/// Destruct.
	virtual ~AWater();

	/// Register factory and attributes.
	static void RegisterObject();


};

}