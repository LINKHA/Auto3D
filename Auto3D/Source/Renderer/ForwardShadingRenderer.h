#pragma once
#include "AutoConfig.h"
#include "Renderer/SceneRenderer.h"
#include "Math/Color.h"
#include <stdint.h>

#include "Component/MeshComponent.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{

class Mesh;

struct GBox
{
	static int64_t _timeOffset;
	static Mesh* _mesh;
	static bgfx::ProgramHandle _program;
	static bgfx::UniformHandle _time;
	static uint32_t _width;
	static uint32_t _height;
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
	void ShutDowm();
private:
	uint32_t _backbufferWidth;
	uint32_t _backbufferHeight;
	/// Renderer debug mode;
	uint32_t _debug;
	/// This flag specifies where flip occurs.
	uint32_t _reset;

	FColor _backbufferColor;
	float _depth;
	uint8_t _stencil;
};

}

