#include "ConstantBuffer.h"
#if AUTO_OPENGL
#include "AutoOGL.h"

#include "NewDef.h"

namespace Auto3D {


void ConstantBuffer::Release()
{
	
}

void ConstantBuffer::OnDeviceReset()
{
	if (_size)
		SetSize(_size); // Recreate
}

bool ConstantBuffer::SetSize(unsigned size)
{
	if (!size)
	{
		WarningString("Can not create zero-sized constant buffer");
		return false;
	}

	// Round up to next 16 bytes
	size += 15;
	size &= 0xfffffff0;

	_size = size;
	_dirty = false;
	_shadowData = SharedArrayPtr<unsigned char>(new unsigned char[_size]);
	memset(_shadowData.get(), 0, _size);

	if (_graphics.lock())
	{
		if (!_object.name)
			glGenBuffers(1, &_object.name);
		_graphics.lock()->SetUBO(_object.name);
		glBufferData(GL_UNIFORM_BUFFER, _size, _shadowData.get(), GL_DYNAMIC_DRAW);
	}

	return true;
}

void ConstantBuffer::Apply()
{
	if (_dirty && _object.name)
	{
		_graphics.lock()->SetUBO(_object.name);
		glBufferData(GL_UNIFORM_BUFFER, _size, _shadowData.get(), GL_DYNAMIC_DRAW);

		_dirty = false;
	}
}


}

#endif