#pragma once
#include "Auto.h"
#include "Object.h"
#include "GPUObject.h"

namespace Auto3D{

class VertexBuffer : public Object , public GPUObject
{
	REGISTER_OBJECT_CLASS(VertexBuffer, Object)
public:
	/**
	* @brief : Construct. Optionally force headless (no GPU-side buffer) operation
	*/
	explicit VertexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless = false);
	
};

}