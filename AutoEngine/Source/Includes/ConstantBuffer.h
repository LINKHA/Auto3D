#pragma once

#include "Object.h"
#include "GPUObject.h"

namespace Auto3D {

class ConstantBuffer : public Object, public GPUObject
{
	REGISTER_OBJECT_CLASS(ConstantBuffer, Object)
public:
	explicit ConstantBuffer(SharedPtr<Ambient> ambient);


};

}