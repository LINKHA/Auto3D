#include "ShaderVariation.h"
#if AUTO_DIRECT_X
#include "AutoD3D12.h"

namespace Auto3D {


bool ShaderVariation::Create()
{
	unsigned int vertex, fragment;

	const char* vsCode = _vsShaderCode.CStr();
	const char* fsCode = _fsShaderCode.CStr();
	const char* gsCode = _gsShaderCode.CStr();

#if defined(AUTO_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif

	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;

	ThrowIfFailed(D3DCompile(vsCode, sizeof(vsCode), nullptr, nullptr, nullptr,
		"VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));

	ThrowIfFailed(D3DCompile(fsCode, sizeof(vsCode), nullptr, nullptr, nullptr,
		"FSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));
	return true;
}

void ShaderVariation::Use()
{
	
}

void ShaderVariation::SetBool(const STRING &name, bool value) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetInt(const STRING &name, int value) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetFloat(const STRING &name, float value) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetVec2(const STRING &name, const glm::vec2 &value) const
{

}
void ShaderVariation::SetVec2(const STRING &name, const Vector2 &value) const
{

}
void ShaderVariation::SetVec2(const STRING &name, float x, float y) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetVec3(const STRING &name, const glm::vec3 &value) const
{

}
void ShaderVariation::SetVec3(const STRING &name, const Color &value) const
{

}
void ShaderVariation::SetVec3(const STRING &name, const Vector3 &value) const
{

}
void ShaderVariation::SetVec3(const STRING &name, float x, float y, float z) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetVec4(const STRING &name, const glm::vec4 &value) const
{

}
void ShaderVariation::SetVec4(const STRING &name, const Vector4 &value) const
{

}
void ShaderVariation::SetVec4(const STRING &name, float x, float y, float z, float w)
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetMat2(const STRING &name, const glm::mat2 &mat) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetMat3(const STRING &name, const glm::mat3 &mat) const
{

}
// ------------------------------------------------------------------------
void ShaderVariation::SetMat4(const STRING &name, const glm::mat4 &mat) const
{

}

void ShaderVariation::checkCompileErrors(GLuint shader, STRING type)
{
	
}



}

#endif