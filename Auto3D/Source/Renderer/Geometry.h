#pragma once
#include "AutoConfig.h"
#include "Container/String.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API FGeometry
{
public:
	FGeometry() 
	{
		_vertexBufferHandles.Resize(0);
		_indexBufferHandles.Resize(0);
		_name = FString::EMPTY;
	}
	~FGeometry() {}

	FString _name;
	TVector<bgfx::VertexBufferHandle> _vertexBufferHandles;
	TVector<bgfx::IndexBufferHandle> _indexBufferHandles;
};

}