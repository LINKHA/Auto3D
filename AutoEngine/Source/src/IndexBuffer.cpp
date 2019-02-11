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

}