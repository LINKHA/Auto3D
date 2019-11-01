#pragma once
#include "Material.h"

namespace Auto3D
{

class IBLMaterial : public Material 
{
	REGISTER_OBJECT_CLASS(IBLMaterial, Material)

public:
	/// Construct.
	IBLMaterial();
	/// Destruct.
	virtual ~IBLMaterial();

	/// Register object factory.
	static void RegisterObject();

	void CreatePass();
};

}