#pragma once
#include "DynamicModel.h"

namespace Auto3D
{

class AUTO_API AWater : public ADynamicModel
{
	DECLARE_CLASS(AWater, ADynamicModel)
public:
	/// Construct.
	AWater();
	/// Destruct.
	virtual ~AWater();

	/// Register factory and attributes.
	static void RegisterObject();


};

}