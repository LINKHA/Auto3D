#pragma once
#include "AutoConfig.h"
#include "Renderer/SceneRenderer.h"
#include "Math/Color.h"
#include <stdint.h>

#include "Component/MeshComponent.h"
#include "Platform/PlatformDef.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class OMesh;
class AWorld;
class ACameraComponent;

struct GBox
{
	static int64_t _timeOffset;
	static SPtr<OMesh> _mesh;
	static bgfx::ProgramHandle _program;
	static bgfx::UniformHandle _time;
	static MouseState _mouseState;
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


	void CollectGeometries(TVector<AActor*>& geometries, AWorld* world, ACameraComponent* camera);

	void ShutDowm();

	void SetBackBufferSize(const TVector2F& size) { _backbufferSize = size; }
	void SetDebugMode(uint32_t debug) { _debug = debug; }
	void SetResetMode(uint32_t reset) { _reset = reset; }
private:
	TVector2F _backbufferSize;
	/// Renderer debug mode;
	uint32_t _debug;
	/// This flag specifies where flip occurs.
	uint32_t _reset;

	FColor _backbufferColor;
	float _depth;
	uint8_t _stencil;

	
};

}

