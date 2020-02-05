#pragma once
#include "AutoConfig.h"
#include "Adapter/Ptr.h"
#include <bgfx/bgfx.h>

namespace Auto3D
{

class OShader;

class AUTO_API FShaderProgram
{
private:
	
	/// Construct with shader pointers.
	FShaderProgram(const SPtr<OShader>& vs, const SPtr<OShader>& ps);
	/// Destruct.
	~FShaderProgram();

	/// Attempt to link the shaders. Return true on success. Note: the shader program is bound if linking is successful.
	bool Link();
	/// Release the linked shader program.
	bool Release();

	/// Return the vertex shader.
	SPtr<OShader> VertexShader() const;
	/// Return the pixel shader.
	SPtr<OShader> PixelShader() const;

	bgfx::ProgramHandle GetShaderProgram() { return _program; }
private:
	/// Shader program identifier.
	bgfx::ProgramHandle _program;
	/// Vertex shader.
	WPtr<OShader> _vs;
	/// Pixel shader.
	WPtr<OShader> _ps;
};

}