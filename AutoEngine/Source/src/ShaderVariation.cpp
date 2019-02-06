#include "ShaderVariation.h"
#include "Shader.h"

namespace Auto3D {


ShaderVariation::ShaderVariation(SharedPtr<Shader> shader)
	: GPUObject(shader->GetSubSystem<Graphics>())
{

	_vsShaderCode = shader->_vsSourceCode;
	_fsShaderCode = shader->_fsSourceCode;
}

ShaderVariation::ShaderVariation(SharedPtr<Shader> vsShader, SharedPtr<Shader> fsShader)
	: GPUObject(vsShader->GetSubSystem<Graphics>())
{
	_vsShaderCode = vsShader->_vsSourceCode;
	_fsShaderCode = fsShader->_fsSourceCode;
}

ShaderVariation::ShaderVariation(SharedPtr<Shader> vsShader, SharedPtr<Shader> fsShader, SharedPtr<Shader> gsShader)
	: GPUObject(vsShader->GetSubSystem<Graphics>())
{
	_vsShaderCode = vsShader->_vsSourceCode;
	_fsShaderCode = fsShader->_fsSourceCode;
	_gsShaderCode = gsShader->_gsSourceCode;

}

ShaderVariation::~ShaderVariation()
{
}

void ShaderVariation::Release()
{

}


}