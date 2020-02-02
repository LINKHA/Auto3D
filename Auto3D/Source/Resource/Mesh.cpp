#include "Resource/Mesh.h"

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>
#include "Platform/PlatformDef.h"
#include <meshoptimizer/src/meshoptimizer.h>
#include "Platform/dbg.h"
#include "Container/String.h"

namespace bgfx
{
	int32_t read(bx::ReaderI* reader, bgfx::VertexLayout& layout, bx::Error* err = NULL);
}

namespace Auto3D
{

void OMesh::load(bx::ReaderSeekerI* reader, bool ramcopy)
{
#define BGFX_CHUNK_MAGIC_VB  BX_MAKEFOURCC('V', 'B', ' ', 0x1)
#define BGFX_CHUNK_MAGIC_VBC BX_MAKEFOURCC('V', 'B', 'C', 0x0)
#define BGFX_CHUNK_MAGIC_IB  BX_MAKEFOURCC('I', 'B', ' ', 0x0)
#define BGFX_CHUNK_MAGIC_IBC BX_MAKEFOURCC('I', 'B', 'C', 0x1)
#define BGFX_CHUNK_MAGIC_PRI BX_MAKEFOURCC('P', 'R', 'I', 0x0)

	using namespace bx;
	using namespace bgfx;

	Group group;

	bx::AllocatorI* allocator = Auto3D::getAllocator();

	uint32_t chunk;
	bx::Error err;
	while (4 == bx::read(reader, chunk, &err)
		&& err.isOk())
	{
		switch (chunk)
		{
		case BGFX_CHUNK_MAGIC_VB:
		{
			read(reader, group._sphere);
			read(reader, group._aabb);
			read(reader, group._obb);

			read(reader, _layout);

			uint16_t stride = _layout.getStride();

			read(reader, group._numVertices);
			const bgfx::Memory* mem = bgfx::alloc(group._numVertices*stride);
			read(reader, mem->data, mem->size);
			if (ramcopy)
			{
				group._vertices = (uint8_t*)BX_ALLOC(allocator, group._numVertices*stride);
				bx::memCopy(group._vertices, mem->data, mem->size);
			}
			group._vbh = bgfx::createVertexBuffer(mem, _layout);
		}
		break;

		case BGFX_CHUNK_MAGIC_VBC:
		{
			read(reader, group._sphere);
			read(reader, group._aabb);
			read(reader, group._obb);

			read(reader, _layout);

			uint16_t stride = _layout.getStride();

			read(reader, group._numVertices);

			const bgfx::Memory* mem = bgfx::alloc(group._numVertices*stride);

			uint32_t compressedSize;
			bx::read(reader, compressedSize);

			void* compressedVertices = BX_ALLOC(allocator, compressedSize);
			bx::read(reader, compressedVertices, compressedSize);

			meshopt_decodeVertexBuffer(mem->data, group._numVertices, stride, (uint8_t*)compressedVertices, compressedSize);

			BX_FREE(allocator, compressedVertices);

			if (ramcopy)
			{
				group._vertices = (uint8_t*)BX_ALLOC(allocator, group._numVertices*stride);
				bx::memCopy(group._vertices, mem->data, mem->size);
			}

			group._vbh = bgfx::createVertexBuffer(mem, _layout);
		}
		break;

		case BGFX_CHUNK_MAGIC_IB:
		{
			read(reader, group._numIndices);
			const bgfx::Memory* mem = bgfx::alloc(group._numIndices * 2);
			read(reader, mem->data, mem->size);
			if (ramcopy)
			{
				group._indices = (uint16_t*)BX_ALLOC(allocator, group._numIndices * 2);
				bx::memCopy(group._indices, mem->data, mem->size);
			}

			group._ibh = bgfx::createIndexBuffer(mem);
		}
		break;

		case BGFX_CHUNK_MAGIC_IBC:
		{
			bx::read(reader, group._numIndices);

			const bgfx::Memory* mem = bgfx::alloc(group._numIndices * 2);

			uint32_t compressedSize;
			bx::read(reader, compressedSize);

			void* compressedIndices = BX_ALLOC(allocator, compressedSize);

			bx::read(reader, compressedIndices, compressedSize);

			meshopt_decodeIndexBuffer(mem->data, group._numIndices, 2, (uint8_t*)compressedIndices, compressedSize);

			BX_FREE(allocator, compressedIndices);

			if (ramcopy)
			{
				group._indices = (uint16_t*)BX_ALLOC(allocator, group._numIndices * 2);
				bx::memCopy(group._indices, mem->data, mem->size);
			}

			group._ibh = bgfx::createIndexBuffer(mem);
		}
		break;

		case BGFX_CHUNK_MAGIC_PRI:
		{
			uint16_t len;
			read(reader, len);

			FString material;
			material.Resize(len);
			read(reader, const_cast<char*>(material.CString()), len);

			uint16_t num;
			read(reader, num);

			for (uint32_t ii = 0; ii < num; ++ii)
			{
				read(reader, len);

				FString name;
				name.Resize(len);
				read(reader, const_cast<char*>(name.CString()), len);

				FPrimitive prim;
				read(reader, prim._startIndex);
				read(reader, prim._numIndices);
				read(reader, prim._startVertex);
				read(reader, prim._numVertices);
				read(reader, prim._sphere);
				read(reader, prim._aabb);
				read(reader, prim._obb);

				group._prims.Push(prim);
			}

			_groups.Push(group);
			group.Reset();
		}
		break;

		default:
			DBG("%08x at %d", chunk, bx::skip(reader, 0));
			break;
		}
	}
}

void OMesh::Load(const char* filePath, bool ramcopy)
{
	bx::FileReaderI* reader = Auto3D::getFileReader();
	if (bx::open(reader, filePath))
	{
		load(reader, ramcopy);
		bx::close(reader);
	}
}
void OMesh::unload()
{
	bx::AllocatorI* allocator = Auto3D::getAllocator();

	for (auto it = _groups.Begin(), itEnd = _groups.End(); it != itEnd; ++it)
	{
		const Group& group = *it;
		bgfx::destroy(group._vbh);

		if (bgfx::isValid(group._ibh))
		{
			bgfx::destroy(group._ibh);
		}

		if (NULL != group._vertices)
		{
			BX_FREE(allocator, group._vertices);
		}

		if (NULL != group._indices)
		{
			BX_FREE(allocator, group._indices);
		}
	}
	_groups.Clear();
}

void OMesh::submit(bgfx::ViewId id, bgfx::ProgramHandle program, const float* mtx, uint64_t state) const
{
	if (BGFX_STATE_MASK == state)
	{
		state = 0
			| BGFX_STATE_WRITE_RGB
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA
			;
	}

	bgfx::setTransform(mtx);
	bgfx::setState(state);

	for (auto it = _groups.Begin(), itEnd = _groups.End(); it != itEnd; ++it)
	{
		const Group& group = *it;

		bgfx::setIndexBuffer(group._ibh);
		bgfx::setVertexBuffer(0, group._vbh);
		bgfx::submit(id, program, 0, it != itEnd - 1);
	}
}

void OMesh::submit(const FMeshState*const* state, uint8_t numPasses, const float* mtx, uint16_t numMatrices) const
{
	uint32_t cached = bgfx::setTransform(mtx, numMatrices);

	for (uint32_t pass = 0; pass < numPasses; ++pass)
	{
		bgfx::setTransform(cached, numMatrices);

		const FMeshState& meshState = *state[pass];
		bgfx::setState(meshState._state);

		for (uint8_t tex = 0; tex < meshState._numTextures; ++tex)
		{
			const FMeshState::Texture& texture = meshState._textures[tex];
			bgfx::setTexture(texture._stage
				, texture._sampler
				, texture._texture
				, texture._flags
			);
		}

		for (auto it = _groups.Begin(), itEnd = _groups.End(); it != itEnd; ++it)
		{
			const Group& group = *it;

			bgfx::setIndexBuffer(group._ibh);
			bgfx::setVertexBuffer(0, group._vbh);
			bgfx::submit(meshState._viewId, meshState._program, 0, it != itEnd - 1);
		}
	}
}

FMeshState* OMesh::meshStateCreate()
{
	FMeshState* state = new FMeshState();
	return state;
}

}