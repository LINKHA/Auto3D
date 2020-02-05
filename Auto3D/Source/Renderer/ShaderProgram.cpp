#include "Renderer/ShaderProgram.h"
#include "Resource/Shader.h"


namespace Auto3D
{

FShaderProgram::FShaderProgram():
	_program(BGFX_INVALID_HANDLE)
{
}

FShaderProgram::FShaderProgram(const SPtr<OShader>& vs, const SPtr<OShader>& ps) :
	_program(BGFX_INVALID_HANDLE)
{
	_vs = vs;
	_ps = ps;
}

FShaderProgram::~FShaderProgram()
{

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

SPtr<OShader> FShaderProgram::GetVertexShader() const
{
	return _vs;
}

SPtr<OShader> FShaderProgram::GetPixelShader() const
{
	return _ps;
}

void FShaderProgram::SetVertexShader(const SPtr<OShader>& shader)
{
	_vs = shader;
}

void FShaderProgram::SetPixelShader(const SPtr<OShader>& shader)
{
	_ps = shader;
}

bool FShaderProgram::CreateVertexShader(const FString& path)
{
	SPtr<OShader> vsShader = MakeShared<OShader>();
	SPtr<OShader> vsShaderss;
	vsShaderss = vsShader;

	bool flag = vsShader->BeginLoad(path);
	flag |= vsShader->EndLoad();

	if (flag && isValid((vsShader->GetShaderHandle())))
	{
		_vs = vsShader;
	}

	return flag;
}

bool FShaderProgram::CreatePixelShader(const FString& path)
{
	SPtr<OShader> psShader = MakeShared<OShader>();
	bool flag = psShader->BeginLoad(path);
	flag |= psShader->EndLoad();

	if (flag && isValid((psShader->GetShaderHandle())))
	{
		_ps = psShader;
	}

	return flag;
}

}