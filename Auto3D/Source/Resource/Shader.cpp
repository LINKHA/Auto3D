#include "Resource/Shader.h"
#include "RHI/bgfx_utils.h"

namespace Auto3D
{

bool OShader::BeginLoad(const FString& pathName)
{
	_shaderHandle = loadShader(pathName.CString());
	if (isValid(_shaderHandle))
	{
		return true;
	}
	return false;
}

bool OShader::EndLoad()
{
	return true;
}

}