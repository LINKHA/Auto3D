#include "ConstantBuffer.h"
#include "Graphics.h"


namespace Auto3D {

ConstantBuffer::ConstantBuffer(SharedPtr<Ambient> ambient) 
	: Object(ambient)
	, GPUObject(GetSubSystem<Graphics>())
{
}

ConstantBuffer::~ConstantBuffer()
{
	
}

}