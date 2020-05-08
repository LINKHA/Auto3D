#pragma once
#include "AutoConfig.h"
#include "Container/String.h"
#include "Container/HashMap.h"
#include "Container/Pair.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{
class FMeshGroup;

class AUTO_API FGeometry
{
public:
	FGeometry() 
	{
		_name = FString::EMPTY;
	}
	~FGeometry() 
	{
	}
	
	FString _name;
	/// The key value of this hashmap is name, which corresponds to MeshGroup and occlusionQuery.
	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle> > _geometryValue;
};

}