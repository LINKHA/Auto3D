#include "IndexBuffer.h"
namespace Auto3D {

IndexBuffer::IndexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless)
	: Super(ambient)
	, GPUObject(forceHeadless ? nullptr : GetSubSystem<Graphics>())
{

}

IndexBuffer::~IndexBuffer() = default;

}