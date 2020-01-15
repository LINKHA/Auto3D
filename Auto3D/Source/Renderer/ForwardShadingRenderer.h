#pragma once
#include "AutoConfig.h"
#include "Renderer/SceneRenderer.h"
#include "Math/Color.h"
#include <stdint.h>

namespace Auto3D
{

/// High-level rendering subsystem. Performs rendering of 3D scenes.
class AUTO_API FForwardShadingRenderer : public ISceneRenderer
{
public:
	/// Construct and register subsystem.
	FForwardShadingRenderer();
	/// Destruct.
	~FForwardShadingRenderer();

	void Init(int32_t argc, const char* const* argv, uint32_t width, uint32_t height)override;
	/// Render scene
	void Render()override;
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

