#pragma once
#include "AutoConfig.h"
#include "Container/Vector.h"
#include "Resource/Resource.h"
#include "Renderer/Geometry.h"

#include <bimg/bimg.h>
#include <bx/pixelformat.h>
#include <bgfx/bgfx.h>
#include "RHI/bounds.h"

namespace Auto3D
{

struct FMeshState
{
	struct Texture
	{
		uint32_t            _flags;
		bgfx::UniformHandle _sampler;
		bgfx::TextureHandle _texture;
		uint8_t             _stage;
	};

	Texture             _textures[4];
	uint64_t            _state;
	bgfx::ProgramHandle _program;
	uint8_t             _numTextures;
	bgfx::ViewId        _viewId;
};

struct FPrimitive
{
	uint32_t _startIndex;
	uint32_t _numIndices;
	uint32_t _startVertex;
	uint32_t _numVertices;

	Sphere _sphere;
	Aabb _aabb;
	Obb _obb;
};

struct FMeshGroup
{
	FMeshGroup()
	{
		Reset();
	}

	void Reset()
	{
		_numVertices = 0;
		_vertices = NULL;
		_numIndices = 0;
		_indices = NULL;
		_vbh.idx = BGFX_INVALID_HANDLE;
		_ibh.idx = BGFX_INVALID_HANDLE;
		_prims.Clear();
	}

	bgfx::VertexBufferHandle _vbh;
	bgfx::IndexBufferHandle _ibh;
	uint16_t _numVertices;
	uint8_t* _vertices;
	uint32_t _numIndices;
	uint16_t* _indices;
	Sphere _sphere;
	Aabb _aabb;
	Obb _obb;
	TVector<FPrimitive> _prims;
};

class AUTO_API OMesh : public OResource
{
	DECLARE_O_CLASS(OMesh, OResource)
public:
	/// Load the resource data from a stream. May be executed outside the main thread, should not access GPU resources. Return true on success.
	virtual bool BeginLoad(const FString& pathName) override;
	/// Finish resource loading if necessary. Always called from the main thread, so GPU resources can be accessed here. Return true on success.
	virtual bool EndLoad()override;


	void unload();

	void submitInstance(bgfx::ViewId id, bgfx::ProgramHandle program, uint64_t state = BGFX_STATE_MASK) const;

	void submit(bgfx::ViewId id, bgfx::ProgramHandle program, const float* mtx, uint64_t state = BGFX_STATE_MASK) const;

	void submit(const FMeshState*const* state, uint8_t numPasses, const float* mtx, uint16_t numMatrices = 1) const;

	void Load(const char* filePath, bool ramcopy = false);

	///
	static FMeshState* meshStateCreate();

	///
	//static void meshStateDestroy(FMeshState* _meshState);

	FGeometry* CreateGeometry();

	bgfx::VertexLayout _layout;
	TVector<FMeshGroup> _groups;
private:
	void PrivateLoad(bx::ReaderSeekerI* reader, bool ramcopy = false);

};


}