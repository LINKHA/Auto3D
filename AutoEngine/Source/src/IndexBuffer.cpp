#include "IndexBuffer.h"

#include "NewDef.h"
namespace Auto3D {

IndexBuffer::IndexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless)
	: Super(ambient)
	, GPUObject(forceHeadless ? nullptr : GetSubSystem<Graphics>())
{

}

IndexBuffer::~IndexBuffer() = default;

void IndexBuffer::OnDeviceLost()
{
	GPUObject::OnDeviceLost();
}

void IndexBuffer::OnDeviceReset()
{

}

void IndexBuffer::Release()
{

}

void IndexBuffer::SetShadowed(bool enable)
{
	// If no graphics subsystem, can not disable shadowing
	if (!_graphics.lock())
		enable = true;

	if (enable != _shadowed)
	{
		if (enable && _indexCount && _indexSize)
			_shadowData = SharedArrayPtr<unsigned char>(new unsigned char[_indexCount * _indexSize]);
		else
			_shadowData.reset();

		_shadowed = enable;
	}
}

bool IndexBuffer::SetSize(unsigned indexCount, bool largeIndices, bool dynamic)
{
	_indexCount = indexCount;
	_indexSize = (unsigned)(largeIndices ? sizeof(unsigned) : sizeof(unsigned short));
	_dynamic = dynamic;

	if (_shadowed && _indexCount && _indexSize)
		_shadowData = SharedArrayPtr<unsigned char>(new unsigned char[_indexCount * _indexSize]);
	else
		_shadowData.reset();

	return create();
}

bool IndexBuffer::GetUsedVertexRange(unsigned start, unsigned count, unsigned& minVertex, unsigned& vertexCount)
{
	if (!_shadowData)
	{
		WarningString("Used vertex range can only be queried from an index buffer with shadow data");
		return false;
	}

	if (start + count > _indexCount)
	{
		WarningString("Illegal index range for querying used vertices");
		return false;
	}

	minVertex = MATH_MAX_UNSIGNED;
	unsigned maxVertex = 0;

	if (_indexSize == sizeof(unsigned))
	{
		unsigned* indices = ((unsigned*)_shadowData.get()) + start;

		for (unsigned i = 0; i < count; ++i)
		{
			if (indices[i] < minVertex)
				minVertex = indices[i];
			if (indices[i] > maxVertex)
				maxVertex = indices[i];
		}
	}
	else
	{
		unsigned short* indices = ((unsigned short*)_shadowData.get()) + start;

		for (unsigned i = 0; i < count; ++i)
		{
			if (indices[i] < minVertex)
				minVertex = indices[i];
			if (indices[i] > maxVertex)
				maxVertex = indices[i];
		}
	}

	vertexCount = maxVertex - minVertex + 1;
	return true;
}

}