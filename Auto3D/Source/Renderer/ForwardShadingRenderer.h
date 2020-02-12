#pragma once
#include "AutoConfig.h"
#include "Renderer/SceneRenderer.h"
#include "Math/Color.h"
#include "Renderer/Batch.h"

#include "Component/MeshComponent.h"
#include "Platform/PlatformDef.h"
#include "Adapter/Ptr.h"

#include <bgfx/bgfx.h>
#include <stdint.h>
namespace Auto3D
{

class OMesh;
class AWorld;
class ACameraComponent;

struct FRenderState
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
struct FShadowMap
{
	FShadowMap() = default;

	FShadowMap(int size):
		_shadowMapFrameBuffer(BGFX_INVALID_HANDLE),
		_size(size)
	{
		_fbtextures[0] = bgfx::createTexture2D(
			_size
			, _size
			, false
			, 1
			, bgfx::TextureFormat::D16
			, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
		);

		_shadowMapFrameBuffer = bgfx::createFrameBuffer(BX_COUNTOF(_fbtextures), _fbtextures, true);
	}
	bgfx::TextureHandle _fbtextures[1];
	bgfx::FrameBufferHandle _shadowMapFrameBuffer;
	uint16_t _size;
	FRenderState _state;
};

/// High-level rendering subsystem. Performs rendering of 3D scenes.
class AUTO_API FForwardShadingRenderer : public ISceneRenderer
{
public:
	/// Construct and register subsystem.
	FForwardShadingRenderer();
	/// Destruct.
	~FForwardShadingRenderer();

	void Init(uint32_t width, uint32_t height)override;
	/// Render scene
	void Render()override;

	void RenderBatches();

	void RenderBatch(FRenderState& renderState);

	void RenderShadowMaps();

	void SetupShadowMaps(size_t num, int size);

	void CollectGeometries(AWorld* world, ACameraComponent* camera);

	void CollectBatch();

	void PrepareView();

	void ShutDowm();

	void SetBackBufferSize(const TVector2F& size) { _backbufferSize = size; }
	void SetDebugMode(uint32_t debug) { _debug = debug; }
	void SetResetMode(uint32_t reset) { _reset = reset; }

	int GetInvisibleBatch() { return _invisibleBatch; }
	int GetVisibleBatch() { return _visibleBatch; }
private:

	/// FBatch queues per pass.
	FRenderQueue _batchQueues;

	/// Geometries in frustum.
	TVector<AActor*> _geometriesActor;

	TVector2F _backbufferSize;
	/// Renderer debug mode;
	uint32_t _debug;
	/// This flag specifies where flip occurs.
	uint32_t _reset;

	FColor _backbufferColor;
	float _depth;
	uint8_t _stencil;

	int _invisibleBatch;
	int _visibleBatch;

	TVector<FShadowMap> _shadowMaps;
};

}

