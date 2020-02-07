#pragma once
#include "AutoConfig.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class AUTO_API FGeometry
{
public:
	FGeometry() 
	{
		_vbh.idx = BGFX_INVALID_HANDLE;
		_ibh.idx = BGFX_INVALID_HANDLE;
	}
	~FGeometry() {}

	bgfx::VertexBufferHandle _vbh;
	bgfx::IndexBufferHandle _ibh;
};

}