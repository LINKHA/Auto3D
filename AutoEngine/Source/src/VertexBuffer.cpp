#include "VertexBuffer.h"

#include "NewDef.h"
namespace Auto3D {

VertexBuffer::VertexBuffer(SharedPtr<Ambient> ambient, bool forceHeadless)
	: Super(ambient)
	, GPUObject(forceHeadless ? nullptr : GetSubSystem<Graphics>())
{
	updateOffsets();
	
	if (!_graphics.lock())
		_shadowed = true;
}

VertexBuffer::~VertexBuffer() = default;

void VertexBuffer::OnDeviceLost()
{
	GPUObject::OnDeviceLost();
}

void VertexBuffer::OnDeviceReset()
{

}

void VertexBuffer::Release()
{

}

void VertexBuffer::SetShadowed(bool enable)
{
	// If no graphics subsystem, can not disable shadowing
	if (!_graphics.lock())
		enable = true;

	if (enable != _shadowed)
	{
		if (enable && _vertexSize && _vertexCount)
			_shadowData = SharedArrayPtr<unsigned char>(new unsigned char[_vertexCount * _vertexSize]);
		else
			_shadowData.reset();

		_shadowed = enable;
	}
}

bool VertexBuffer::SetSize(unsigned vertexCount, unsigned elementMask, bool dynamic)
{
	return SetSize(vertexCount, GetElements(elementMask), dynamic);
}

bool VertexBuffer::SetSize(unsigned vertexCount, const VECTOR<VertexElement>& elements, bool dynamic)
{
	_vertexCount = vertexCount;
	_elements = elements;
	_dynamic = dynamic;

	updateOffsets();

	if (_shadowed && _vertexCount && _vertexSize)
		_shadowData = SharedArrayPtr<unsigned char>(new unsigned char[_vertexCount * _vertexSize]);
	else
		_shadowData.reset();

	return create();
}

VECTOR<VertexElement> VertexBuffer::GetElements(unsigned elementMask)
{
	VECTOR<VertexElement> ret;

	for (unsigned i = 0; i < (unsigned)LegacyVertexElement::Count; ++i)
	{
		if (elementMask & (1u << i))
			ret.push_back(LEGACY_VERTEXELEMENTS[i]);
	}

	return ret;
}

void VertexBuffer::updateOffsets()
{
	unsigned elementOffset = 0;
	_elementHash = 0;
	_elementMask = VERTEX_MASK_NONE;

	for (VECTOR<VertexElement>::iterator i = _elements.begin(); i != _elements.end(); ++i)
	{
		i->_offset = elementOffset;
		elementOffset += ELEMENT_TYPESIZES[(int)i->_type];
		_elementHash <<= 6;
		_elementHash += (((int)i->_type + 1) * ((int)i->_semantic + 1) + i->_index);

		for (unsigned j = 0; j < (unsigned)LegacyVertexElement::Count; ++j)
		{
			const VertexElement& legacy = LEGACY_VERTEXELEMENTS[j];
			if (i->_type == legacy._type && i->_semantic == legacy._semantic && i->_index == legacy._index)
				_elementMask |= VertexMaskFlags(1u << j);
		}
	}

	_vertexSize = elementOffset;
}


}