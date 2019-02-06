#include "VertexBuffer.h"
namespace Auto3D {

VertexBuffer::VertexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless)
	: Super(ambient)
	, GPUObject(forceHeadless ? nullptr : GetSubSystem<Graphics>())
{

}

VertexBuffer::~VertexBuffer() = default;

}