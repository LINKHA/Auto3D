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
	FShaderProgram(OShader* vs, OShader* ps);
	/// Destruct.
	~FShaderProgram();

	/// Attempt to link the shaders. Return true on success. Note: the shader program is bound if linking is successful.
	bool Link();
	/// Release the linked shader program.
	bool Release();

	/// Return the vertex shader.
	OShader* GetVertexShader() const;
	/// Return the pixel shader.
	OShader* GetPixelShader() const;

	bgfx::ProgramHandle GetProgram() { return _program; }

	void SetVertexShader(OShader* shader);
	void SetPixelShader(OShader* shader);

	bool CreateVertexShader(const FString& path);
	bool CreatePixelShader(const FString& path);
private:
	/// Shader program identifier.
	bgfx::ProgramHandle _program;
	/// Vertex shader.
	OShader* _vs;
	/// Pixel shader.
	OShader* _ps;
};

}