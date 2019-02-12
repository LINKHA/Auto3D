#include "Geometry.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace Auto3D {

Geometry::Geometry(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _primitiveType(PrimitiveType::TringleList)
	, _indexStart(0)
	, _indexCount(0)
	, _vertexStart(0)
	, _vertexCount(0)
	, _rawVertexSize(0)
	, _rawIndexSize(0)
	, _lodDistance(0.0f)
{
	SetNumVertexBuffers(1);
}

Geometry::~Geometry()
{

}

bool Geometry::SetNumVertexBuffers(unsigned num)
{
	if (num >= MAX_VERTEX_STREAMS)
	{
		WarningString("Too many vertex streams");
		return false;
	}

	unsigned oldSize = _vertexBuffers.size();
	_vertexBuffers.resize(num);

	return true;
}

bool Geometry::SetVertexBuffer(unsigned index, SharedPtr<VertexBuffer> buffer)
{
	if (index >= _vertexBuffers.size())
	{
		WarningString("Stream index out of bounds");
		return false;
	}

	_vertexBuffers[index] = buffer;
	return true;
}

void Geometry::SetIndexBuffer(SharedPtr<IndexBuffer> buffer)
{
	_indexBuffer = buffer;
}

bool Geometry::SetDrawRange(PrimitiveType type, unsigned indexStart, unsigned indexCount, bool getUsedVertexRange)
{
	if (!_indexBuffer && !_rawIndexData)
	{
		WarningString("Null index buffer and no raw index data, can not define indexed draw range");
		return false;
	}
	if (_indexBuffer && indexStart + indexCount > _indexBuffer->GetIndexCount())
	{
		WarningString("Illegal draw range " + STRING(indexStart) + " to " + STRING(indexStart + indexCount - 1) + ", index buffer has " +
			STRING(_indexBuffer->GetIndexCount()) + " indices");
		return false;
	}

	_primitiveType = type;
	_indexStart = indexStart;
	_indexCount = indexCount;

	// Get min.vertex index and num of vertices from index buffer. If it fails, use full range as fallback
	if (indexCount)
	{
		_vertexStart = 0;
		_vertexCount = _vertexBuffers[0] ? _vertexBuffers[0]->GetVertexCount() : 0;

		if (getUsedVertexRange && _indexBuffer)
			_indexBuffer->GetUsedVertexRange(_indexStart, _indexCount, _vertexStart, _vertexCount);
	}
	else
	{
		_vertexStart = 0;
		_vertexCount = 0;
	}

	return true;
}

bool Geometry::SetDrawRange(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned vertexStart, unsigned vertexCount,
	bool checkIllegal)
{
	if (_indexBuffer)
	{
		// We can allow setting an illegal draw range now if the caller guarantees to resize / fill the buffer later
		if (checkIllegal && indexStart + indexCount > _indexBuffer->GetIndexCount())
		{
			STRING("Illegal draw range " + STRING(indexStart) + " to " + STRING(indexStart + indexCount - 1) +
				", index buffer has " + STRING(_indexBuffer->GetIndexCount()) + " indices");
			return false;
		}
	}
	else if (!_rawIndexData)
	{
		indexStart = 0;
		indexCount = 0;
	}

	_primitiveType = type;
	_indexStart = indexStart;
	_indexCount = indexCount;
	_vertexStart = vertexStart;
	_vertexCount = vertexCount;

	return true;
}


}