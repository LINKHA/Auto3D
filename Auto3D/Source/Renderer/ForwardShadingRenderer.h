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

	void CollectGeometries(AWorld* world, ACameraComponent* camera);

	void CollectBatch();

	void CollectLight(AWorld* world, ACameraComponent* camera);

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
	/// Light actor.
	TVector<AActor*> _lightActor;

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
};

}

