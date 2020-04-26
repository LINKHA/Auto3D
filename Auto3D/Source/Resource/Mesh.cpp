#include "Resource/Mesh.h"

#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>
#include <bx/timer.h>

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

bool OMesh::BeginLoad(const FString& pathName)
{
	bx::FileReaderI* reader = Auto3D::FDefaultFileWriterReader::GetFileReader();
	if (bx::open(reader, pathName.CString()))
	{
		PrivateLoad(reader);
		bx::close(reader);
		return true;
	}
	return false;
}

bool OMesh::EndLoad()
{
	return true;
}

void OMesh::PrivateLoad(bx::ReaderSeekerI* reader, bool ramcopy)
{
#define BGFX_CHUNK_MAGIC_VB  BX_MAKEFOURCC('V', 'B', ' ', 0x1)
#define BGFX_CHUNK_MAGIC_VBC BX_MAKEFOURCC('V', 'B', 'C', 0x0)
#define BGFX_CHUNK_MAGIC_IB  BX_MAKEFOURCC('I', 'B', ' ', 0x0)
#define BGFX_CHUNK_MAGIC_IBC BX_MAKEFOURCC('I', 'B', 'C', 0x1)
#define BGFX_CHUNK_MAGIC_PRI BX_MAKEFOURCC('P', 'R', 'I', 0x0)

	using namespace bx;
	using namespace bgfx;

	FMeshGroup group;

	bx::AllocatorI* allocator = Auto3D::FDefaultFileWriterReader::GetAllocator();

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
	bx::FileReaderI* reader = Auto3D::FDefaultFileWriterReader::GetFileReader();
	if (bx::open(reader, filePath))
	{
		PrivateLoad(reader, ramcopy);
		bx::close(reader);
	}
}
void OMesh::unload()
{
	bx::AllocatorI* allocator = Auto3D::FDefaultFileWriterReader::GetAllocator();

	for (auto it = _groups.Begin(), itEnd = _groups.End(); it != itEnd; ++it)
	{
		const FMeshGroup& group = *it;
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

void OMesh::submitInstance(bgfx::ViewId id, bgfx::ProgramHandle program, uint64_t state) const
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

	//////////////////////////////////////////////////////////////////////////
	// 80 bytes stride = 64 bytes for 4x4 matrix + 16 bytes for RGBA color.
	const uint16_t instanceStride = 64;
	// 11x11 cubes
	const uint32_t numInstances = 121;

	float time = (float)((bx::getHPCounter()) / double(bx::getHPFrequency()));

	bgfx::InstanceDataBuffer idb;
	bgfx::allocInstanceDataBuffer(&idb, numInstances, instanceStride);
	uint8_t* data = idb.data;

	// Write instance data for 11x11 cubes.
	for (uint32_t yy = 0; yy < 11; ++yy)
	{
		for (uint32_t xx = 0; xx < 11; ++xx)
		{
			float* mtx = (float*)data;
			bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
			mtx[12] = -15.0f + float(xx)*3.0f;
			mtx[13] = -15.0f + float(yy)*3.0f;
			mtx[14] = 0.0f;

			/*float* color = (float*)&data[64];
			color[0] = bx::sin(time + float(xx) / 11.0f)*0.5f + 0.5f;
			color[1] = bx::cos(time + float(yy) / 11.0f)*0.5f + 0.5f;
			color[2] = bx::sin(time*3.0f)*0.5f + 0.5f;
			color[3] = 1.0f;*/

			data += instanceStride;
		}
	}

	bgfx::setState(state);

	for (auto it = _groups.Begin(), itEnd = _groups.End(); it != itEnd; ++it)
	{
		const FMeshGroup& group = *it;

		bgfx::setIndexBuffer(group._ibh);
		bgfx::setVertexBuffer(0, group._vbh);

		// Set instance data buffer.
		bgfx::setInstanceDataBuffer(&idb);

		bgfx::submit(id, program, 0, it != itEnd - 1);
	}
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
		const FMeshGroup& group = *it;

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
			const FMeshGroup& group = *it;

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

FGeometry* OMesh::CreateGeometry()
{
	FGeometry* geometry = new FGeometry;
	geometry->_name = GetPathName();
	geometry->_mesh = this;
	/*for (auto it = _groups.Begin(); it != _groups.End(); ++it)
	{
		const FMeshGroup& group = *it;

		geometry->_vertexBufferHandles.Push(group._vbh);
		geometry->_indexBufferHandles.Push(group._ibh);
	}*/

	return geometry;
}

}