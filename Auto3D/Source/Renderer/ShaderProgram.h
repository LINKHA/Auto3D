#pragma once
#include "AutoConfig.h"
#include "Adapter/Ptr.h"
#include "Container/String.h"

#include <bgfx/bgfx.h>
namespace Auto3D
{

class OShader;

class AUTO_API FShaderProgram
{
public:
	/// Construct with shader pointers.
	FShaderProgram();
	/// Construct with shader pointers.
	FShaderProgram(const SPtr<OShader>& vs, const SPtr<OShader>& ps);
	/// Destruct.
	~FShaderProgram();

	/// Attempt to link the shaders. Return true on success. Note: the shader program is bound if linking is successful.
	bool Link();
	/// Release the linked shader program.
	bool Release();

	/// Return the vertex shader.
	SPtr<OShader> GetVertexShader() const;
	/// Return the pixel shader.
	SPtr<OShader> GetPixelShader() const;

	bgfx::ProgramHandle GetShaderProgram() { return _program; }

	void SetVertexShader(const SPtr<OShader>& shader);
	void SetPixelShader(const SPtr<OShader>& shader);

	bool CreateVertexShader(const FString& path);
	bool CreatePixelShader(const FString& path);
private:
	/// Shader program identifier.
	bgfx::ProgramHandle _program;
	/// Vertex shader.
	SPtr<OShader> _vs;
	/// Pixel shader.
	SPtr<OShader> _ps;
};

}