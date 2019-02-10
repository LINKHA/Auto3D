#pragma once

#include "Object.h"
#include "GPUObject.h"
#include "GraphicsDef.h"

namespace Auto3D
{
/**
* Hardware index buffer
*/
class IndexBuffer : public Object, public GPUObject
{
	REGISTER_OBJECT_CLASS(IndexBuffer, Object)
public:
	/// Construct. Optionally force headless (no GPU-side buffer) operation.
	explicit IndexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless = false);
};

}
