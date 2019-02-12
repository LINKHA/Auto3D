#pragma once
#include "Auto.h"
#include "Object.h"
#include "GraphicsDef.h"

namespace Auto3D {

class VertexBuffer;
class IndexBuffer;

class Geometry : public Object
{
	REGISTER_OBJECT_CLASS(Geometry, Object)
public:
	explicit Geometry(SharedPtr<Ambient> ambient);

	/// Set number of vertex buffers
	bool SetNumVertexBuffers(unsigned num);
	/// Set a vertex buffer by index
	bool SetVertexBuffer(unsigned index, SharedPtr<VertexBuffer> buffer);
	/// Set the index buffer
	void SetIndexBuffer(SharedPtr<IndexBuffer> buffer);
	/// Set the draw range
	bool SetDrawRange(PrimitiveType type, unsigned indexStart, unsigned indexCount, bool getUsedVertexRange = true);
	/// Set the draw range
	bool SetDrawRange(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned vertexStart, unsigned vertexCount,
		bool checkIllegal = true);
private:
	/// Vertex buffers
	VECTOR<SharedPtr<VertexBuffer> > _vertexBuffers;
	/// Index buffer
	SharedPtr<IndexBuffer> _indexBuffer;
	/// Primitive type
	PrimitiveType _primitiveType;
	/// Start index
	unsigned _indexStart;
	/// Number of indices
	unsigned _indexCount;
	/// First used vertex
	unsigned _vertexStart;
	/// Number of used vertices
	unsigned _vertexCount;
	/// LOD distance
	float _lodDistance;
	/// Raw vertex data elements
	VECTOR<VertexElement> _rawElements;
	/// Raw vertex data override
	SharedArrayPtr<unsigned char> _rawVertexData;
	/// Raw index data override
	SharedArrayPtr<unsigned char> _rawIndexData;
	/// Raw vertex data override size
	unsigned _rawVertexSize;
	/// Raw index data override size
	unsigned _rawIndexSize;
};

}