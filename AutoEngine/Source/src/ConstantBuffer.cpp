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
	Release();
}

void ConstantBuffer::SetParameter(unsigned offset, unsigned size, const void* data)
{
	if (offset + size > _size)
		return; // Would overflow the buffer

	memcpy(&_shadowData[offset], data, size);
	_dirty = true;
}

void ConstantBuffer::SetVector3ArrayParameter(unsigned offset, unsigned rows, const void* data)
{
	if (offset + rows * 4 * sizeof(float) > _size)
		return; // Would overflow the buffer

	auto* dest = (float*)&_shadowData[offset];
	const auto* src = (const float*)data;

	while (rows--)
	{
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		++dest; // Skip over the w coordinate
	}

	_dirty = true;
}

}