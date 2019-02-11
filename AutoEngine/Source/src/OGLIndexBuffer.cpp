#include "IndexBuffer.h"
#if AUTO_OPENGL
#include "AutoOGL.h"

namespace Auto3D {

bool IndexBuffer::SetData(const void* data)
{
	if (!data)
	{
		WarningString("Null pointer for index buffer data");
		return false;
	}

	if (!_indexSize)
	{
		WarningString("Index size not defined, can not set index buffer data");
		return false;
	}

	if (_shadowData && data != _shadowData.get())
		memcpy(_shadowData.get(), data, _indexCount * (size_t)_indexSize);

	if (_object.name)
	{
		if (!_graphics.lock()->IsDeviceLost())
		{
			_graphics.lock()->SetIndexBuffer(SharedFromThis());
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * (size_t)_indexSize, data, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		}
		else
		{
			WarningString("Index buffer data assignment while device is lost");
			_dataPending = true;
		}
	}

	_dataLost = false;
	return true;
}
	
bool IndexBuffer::create()
{
	if (!_indexCount)
	{
		Release();
		return true;
	}

	if (_graphics.lock())
	{
		if (_graphics.lock()->IsDeviceLost())
		{
			WarningString("Index buffer creation while device is lost");
			return true;
		}

		if (!_object.name)
			glGenBuffers(1, &_object.name);
		if (!_object.name)
		{
			ErrorString("Failed to create index buffer");
			return false;
		}

		_graphics.lock()->SetIndexBuffer(SharedFromThis());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * (size_t)_indexSize, nullptr, _dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}
	return true;
}

}
#endif