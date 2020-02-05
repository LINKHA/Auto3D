#include "Renderer/ShaderProgram.h"
#include "Resource/Shader.h"

namespace Auto3D
{

FShaderProgram::FShaderProgram(const SPtr<OShader>& vs, const SPtr<OShader>& ps)
{
	_program = BGFX_INVALID_HANDLE;
	_vs = vs;
	_ps = ps;
}

FShaderProgram::~FShaderProgram()
{

}

bool FShaderProgram::Link()
{
	bgfx::ShaderHandle vsh = _vs.lock()->GetShaderHandle();
	bgfx::ShaderHandle psh = BGFX_INVALID_HANDLE;
	if (_ps.lock())
	{
		psh = _ps.lock()->GetShaderHandle();
	}
	_program = bgfx::createProgram(vsh, psh, true /* destroy shaders when program is destroyed */);

	return true;
}

bool FShaderProgram::Release()
{
	bgfx::destroy(_program);

	return true;
}


}