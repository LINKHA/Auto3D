#include "VertexBuffer.h"
#if AUTO_OPENGL
#include "AutoOGL.h"

namespace Auto3D {

bool VertexBuffer::SetData(const void* data)
{
	if (!data)
	{
		WarningString("Null pointer for vertex buffer data");
		return false;
	}

	if (!_vertexSize)
	{
		WarningString("Vertex elements not defined, can not set vertex buffer data");
		return false;
	}

	if (_shadowData && data != _shadowData.get())
		memcpy(_shadowData.get(), data, _vertexCount * (size_t)_vertexSize);

	if (_object.name)
	{
		if (!_graphics.lock()->IsDeviceLost())
		{
			_graphics.lock()->SetVBO(_object.name);
			glBufferData(GL_ARRAY_BUFFER, _vertexCount * (size_t)_vertexSize, data, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			WarningString("Vertex buffer data assignment while device is lost");
			_dataPending = true;
		}
	}

	_dataLost = false;
	return true;
}

bool VertexBuffer::create()
{
	if (!_vertexCount || !_elementMask)
	{
		Release();
		return true;
	}

	if (_graphics.lock())
	{
		if (_graphics.lock()->IsDeviceLost())
		{
			WarningString("Vertex buffer creation while device is lost");
			return true;
		}

		if (!_object.name)
			glGenBuffers(1, &_object.name);
		if (!_object.name)
		{
			ErrorString("Failed to create vertex buffer");
			return false;
		}

		_graphics.lock()->SetVBO(_object.name);
		glBufferData(GL_ARRAY_BUFFER, _vertexCount * (size_t)_vertexSize, nullptr, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	return true;
}

}

#endif