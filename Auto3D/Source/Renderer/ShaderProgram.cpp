#include "Renderer/ShaderProgram.h"
#include "Resource/Shader.h"
#include "Resource/ResourceCache.h"

namespace Auto3D
{

FShaderProgram::FShaderProgram():
	_program(BGFX_INVALID_HANDLE)
{
}

FShaderProgram::FShaderProgram(OShader* vs, OShader* ps) :
	_program(BGFX_INVALID_HANDLE)
{
	_vs = vs;
	_ps = ps;
}

FShaderProgram::~FShaderProgram()
{
	if (isValid(_program))
	{
		bgfx::destroy(_program);
	}
}

bool FShaderProgram::Link()
{
	bgfx::ShaderHandle vsh = _vs->GetShaderHandle();
	bgfx::ShaderHandle psh = BGFX_INVALID_HANDLE;
	if (_ps)
	{
		psh = _ps->GetShaderHandle();
	}
	_program = bgfx::createProgram(vsh, psh, true /* destroy shaders when program is destroyed */);

	return true;
}

bool FShaderProgram::Release()
{
	bgfx::destroy(_program);

	return true;
}

OShader* FShaderProgram::GetVertexShader() const
{
	return _vs;
}

OShader* FShaderProgram::GetPixelShader() const
{
	return _ps;
}

void FShaderProgram::SetVertexShader(OShader* shader)
{
	_vs = shader;
}

void FShaderProgram::SetPixelShader(OShader* shader)
{
	_ps = shader;
}

bool FShaderProgram::CreateVertexShader(const FString& path)
{
	OShader* vsShader = GResourceModule::Get().LoadResource<OShader>(path);
	if (vsShader && isValid((vsShader->GetShaderHandle())))
	{
		_vs = vsShader;
	}
	else
		return false;

	return true;
}

bool FShaderProgram::CreatePixelShader(const FString& path)
{
	OShader* psShader = GResourceModule::Get().LoadResource<OShader>(path);
	if (psShader && isValid((psShader->GetShaderHandle())))
	{
		_ps = psShader;
	}
	else
		return false;

	return true;
}

}