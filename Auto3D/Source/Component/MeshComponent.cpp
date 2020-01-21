#include "Component/MeshComponent.h"
#include <bgfx/bgfx.h>
#include <bx/readerwriter.h>
#include "Platform/PlatformDef.h"
#include <meshoptimizer/src/meshoptimizer.h>
#include "Platform/dbg.h"
#include "Container/String.h"
namespace bgfx
{
	int32_t read(bx::ReaderI* _reader, bgfx::VertexLayout& _layout, bx::Error* _err = NULL);
}

namespace Auto3D
{

	Group::Group()
	{
		reset();
	}

	void Group::reset()
	{
		m_vbh.idx = bgfx::kInvalidHandle;
		m_ibh.idx = bgfx::kInvalidHandle;
		m_numVertices = 0;
		m_vertices = NULL;
		m_numIndices = 0;
		m_indices = NULL;
		m_prims.Clear();
	}

	
	void Mesh::load(bx::ReaderSeekerI* _reader, bool _ramcopy)
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
		while (4 == bx::read(_reader, chunk, &err)
			&& err.isOk())
		{
			switch (chunk)
			{
			case BGFX_CHUNK_MAGIC_VB:
			{
				read(_reader, group.m_sphere);
				read(_reader, group.m_aabb);
				read(_reader, group.m_obb);

				read(_reader, m_layout);

				uint16_t stride = m_layout.getStride();

				read(_reader, group.m_numVertices);
				const bgfx::Memory* mem = bgfx::alloc(group.m_numVertices*stride);
				read(_reader, mem->data, mem->size);
				if (_ramcopy)
				{
					group.m_vertices = (uint8_t*)BX_ALLOC(allocator, group.m_numVertices*stride);
					bx::memCopy(group.m_vertices, mem->data, mem->size);
				}
				group.m_vbh = bgfx::createVertexBuffer(mem, m_layout);
			}
			break;

			case BGFX_CHUNK_MAGIC_VBC:
			{
				read(_reader, group.m_sphere);
				read(_reader, group.m_aabb);
				read(_reader, group.m_obb);

				read(_reader, m_layout);

				uint16_t stride = m_layout.getStride();

				read(_reader, group.m_numVertices);

				const bgfx::Memory* mem = bgfx::alloc(group.m_numVertices*stride);

				uint32_t compressedSize;
				bx::read(_reader, compressedSize);

				void* compressedVertices = BX_ALLOC(allocator, compressedSize);
				bx::read(_reader, compressedVertices, compressedSize);

				meshopt_decodeVertexBuffer(mem->data, group.m_numVertices, stride, (uint8_t*)compressedVertices, compressedSize);

				BX_FREE(allocator, compressedVertices);

				if (_ramcopy)
				{
					group.m_vertices = (uint8_t*)BX_ALLOC(allocator, group.m_numVertices*stride);
					bx::memCopy(group.m_vertices, mem->data, mem->size);
				}

				group.m_vbh = bgfx::createVertexBuffer(mem, m_layout);
			}
			break;

			case BGFX_CHUNK_MAGIC_IB:
			{
				read(_reader, group.m_numIndices);
				const bgfx::Memory* mem = bgfx::alloc(group.m_numIndices * 2);
				read(_reader, mem->data, mem->size);
				if (_ramcopy)
				{
					group.m_indices = (uint16_t*)BX_ALLOC(allocator, group.m_numIndices * 2);
					bx::memCopy(group.m_indices, mem->data, mem->size);
				}

				group.m_ibh = bgfx::createIndexBuffer(mem);
			}
			break;

			case BGFX_CHUNK_MAGIC_IBC:
			{
				bx::read(_reader, group.m_numIndices);

				const bgfx::Memory* mem = bgfx::alloc(group.m_numIndices * 2);

				uint32_t compressedSize;
				bx::read(_reader, compressedSize);

				void* compressedIndices = BX_ALLOC(allocator, compressedSize);

				bx::read(_reader, compressedIndices, compressedSize);

				meshopt_decodeIndexBuffer(mem->data, group.m_numIndices, 2, (uint8_t*)compressedIndices, compressedSize);

				BX_FREE(allocator, compressedIndices);

				if (_ramcopy)
				{
					group.m_indices = (uint16_t*)BX_ALLOC(allocator, group.m_numIndices * 2);
					bx::memCopy(group.m_indices, mem->data, mem->size);
				}

				group.m_ibh = bgfx::createIndexBuffer(mem);
			}
			break;

			case BGFX_CHUNK_MAGIC_PRI:
			{
				uint16_t len;
				read(_reader, len);

				FString material;
				material.Resize(len);
				read(_reader, const_cast<char*>(material.CString()), len);

				uint16_t num;
				read(_reader, num);

				for (uint32_t ii = 0; ii < num; ++ii)
				{
					read(_reader, len);

					FString name;
					name.Resize(len);
					read(_reader, const_cast<char*>(name.CString()), len);

					Primitive prim;
					read(_reader, prim.m_startIndex);
					read(_reader, prim.m_numIndices);
					read(_reader, prim.m_startVertex);
					read(_reader, prim.m_numVertices);
					read(_reader, prim.m_sphere);
					read(_reader, prim.m_aabb);
					read(_reader, prim.m_obb);

					group.m_prims.Push(prim);
				}

				m_groups.Push(group);
				group.reset();
			}
			break;

			default:
				DBG("%08x at %d", chunk, bx::skip(_reader, 0));
				break;
			}
		}
	}

	void Mesh::unload()
	{
		bx::AllocatorI* allocator = Auto3D::getAllocator();

		for (auto it = m_groups.Begin(), itEnd = m_groups.End(); it != itEnd; ++it)
		{
			const Group& group = *it;
			bgfx::destroy(group.m_vbh);

			if (bgfx::isValid(group.m_ibh))
			{
				bgfx::destroy(group.m_ibh);
			}

			if (NULL != group.m_vertices)
			{
				BX_FREE(allocator, group.m_vertices);
			}

			if (NULL != group.m_indices)
			{
				BX_FREE(allocator, group.m_indices);
			}
		}
		m_groups.Clear();
	}

	void Mesh::submit(bgfx::ViewId _id, bgfx::ProgramHandle _program, const float* _mtx, uint64_t _state) const
	{
		if (BGFX_STATE_MASK == _state)
		{
			_state = 0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				;
		}

		bgfx::setTransform(_mtx);
		bgfx::setState(_state);

		for (auto it = m_groups.Begin(), itEnd = m_groups.End(); it != itEnd; ++it)
		{
			const Group& group = *it;

			bgfx::setIndexBuffer(group.m_ibh);
			bgfx::setVertexBuffer(0, group.m_vbh);
			bgfx::submit(_id, _program, 0, it != itEnd - 1);
		}
	}

	void Mesh::submit(const MeshState*const* _state, uint8_t _numPasses, const float* _mtx, uint16_t _numMatrices) const
	{
		uint32_t cached = bgfx::setTransform(_mtx, _numMatrices);

		for (uint32_t pass = 0; pass < _numPasses; ++pass)
		{
			bgfx::setTransform(cached, _numMatrices);

			const MeshState& state = *_state[pass];
			bgfx::setState(state.m_state);

			for (uint8_t tex = 0; tex < state.m_numTextures; ++tex)
			{
				const MeshState::Texture& texture = state.m_textures[tex];
				bgfx::setTexture(texture.m_stage
					, texture.m_sampler
					, texture.m_texture
					, texture.m_flags
				);
			}

			for (auto it = m_groups.Begin(), itEnd = m_groups.End(); it != itEnd; ++it)
			{
				const Group& group = *it;

				bgfx::setIndexBuffer(group.m_ibh);
				bgfx::setVertexBuffer(0, group.m_vbh);
				bgfx::submit(state.m_viewId, state.m_program, 0, it != itEnd - 1);
			}
		}
	}

	Mesh* meshLoad(bx::ReaderSeekerI* _reader, bool _ramcopy)
	{
		Mesh* mesh = new Mesh;
		mesh->load(_reader, _ramcopy);
		return mesh;
	}

	Mesh* meshLoad(const char* _filePath, bool _ramcopy)
	{
		bx::FileReaderI* reader = Auto3D::getFileReader();
		if (bx::open(reader, _filePath))
		{
			Mesh* mesh = meshLoad(reader, _ramcopy);
			bx::close(reader);
			return mesh;
		}

		return NULL;
	}

	void meshUnload(Mesh* _mesh)
	{
		_mesh->unload();
		delete _mesh;
	}

	MeshState* meshStateCreate()
	{
		MeshState* state = (MeshState*)BX_ALLOC(Auto3D::getAllocator(), sizeof(MeshState));
		return state;
	}

	void meshStateDestroy(MeshState* _meshState)
	{
		BX_FREE(Auto3D::getAllocator(), _meshState);
	}

	void meshSubmit(const Mesh* _mesh, bgfx::ViewId _id, bgfx::ProgramHandle _program, const float* _mtx, uint64_t _state)
	{
		_mesh->submit(_id, _program, _mtx, _state);
	}

	void meshSubmit(const Mesh* _mesh, const MeshState*const* _state, uint8_t _numPasses, const float* _mtx, uint16_t _numMatrices)
	{
		_mesh->submit(_state, _numPasses, _mtx, _numMatrices);
	}

}