#pragma once

#include "../../Container/AutoPtr.h"
#include "../../Container/Vector.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"

namespace Auto3D
{

/// GPU buffer for vertex data.
class AUTO_API FVertexBuffer : public FRefCounted, public FGPUObject
{
public:
	/// Construct.
	FVertexBuffer();
	/// Destruct.
	~FVertexBuffer();

	/// Release the vertex buffer and CPU shadow data.
	void Release() override;
	/// Recreate the GPU resource after data loss.
	void Recreate() override;

	/// Define buffer. Immutable buffers must specify initial data here. Return true on success.
	bool Define(EResourceUsage::Type usage, size_t numVertices, const TVector<FVertexElement>& elements, bool useShadowData, const void* _data = nullptr);
	/// Define buffer. Immutable buffers must specify initial data here. Return true on success.
	bool Define(EResourceUsage::Type usage, size_t numVertices, size_t numElements, const FVertexElement* elements, bool useShadowData, const void* _data = nullptr);
	/// Redefine buffer data either completely or partially. Not supported for immutable buffers. Return true on success.
	bool SetData(size_t firstVertex, size_t numVertices, const void* _data);

	/// Return CPU-side shadow data if exists.
	unsigned char* ShadowData() const { return _shadowData.Get(); }
	/// Return number of vertices.
	size_t GetNumVertices() const { return _numVertices; }
	/// Return number of vertex elements.
	size_t GetNumElements() const { return _elements.Size(); }
	/// Return vertex elements.
	const TVector<FVertexElement>& GetElements() const { return _elements; }
	/// Return _size of vertex in bytes.
	size_t GetVertexSize() const { return _vertexSize; }
	/// Return vertex declaration hash code.
	unsigned GetElementHash() const { return _elementHash; }
	/// Return resource usage type.
	EResourceUsage::Type GetUsage() const { return _usage; }
	/// Return whether is dynamic.
	bool IsDynamic() const { return _usage == EResourceUsage::DYNAMIC; }
	/// Return whether is immutable.
	bool IsImmutable() const { return _usage == EResourceUsage::IMMUTABLE; }

	/// Return the OpenGL buffer identifier. Used internally and should not be called by portable application code.
	unsigned GetGLBuffer() const { return _buffer; }

	/// Compute the hash code of one vertex element by index and semantic.
	static unsigned ElementHash(size_t index, EElementSemantic::Type semantic) { return (semantic + 1) << (index * 3); }

	/// Vertex element D3D11 format by element type.
	static const unsigned elementFormats[];
	/// Vertex element semantic names.
	static const char* elementSemanticNames[];

private:
	/// Create the GPU-side vertex buffer. Return true on success.
	bool Create(const void* _data);

	/// OpenGL buffer object identifier.
	unsigned _buffer;
	/// CPU-side shadow data.
	TAutoArrayPtr<unsigned char> _shadowData;
	/// Number of vertices.
	size_t _numVertices;
	/// Size of vertex in bytes.
	size_t _vertexSize;
	/// Vertex elements.
	TVector<FVertexElement> _elements;
	/// Vertex element hash code.
	unsigned _elementHash;
	/// AResource usage type.
	EResourceUsage::Type _usage;
};

}