#include "OGLShaderProgram.h"
#if AUTO_OPENGL 
#include "AutoOGL.h"
#include "ShaderVariation.h"
namespace Auto3D {


ShaderProgram::ShaderProgram(SharedPtr<Graphics> graphics, SharedPtr<ShaderVariation> vs, SharedPtr<ShaderVariation> ps)
	: GPUObject(graphics)
	, _program(0)
	, _vs(vs)
	, _ps(ps)
{
}

ShaderProgram::~ShaderProgram()
{
	Release();
}

void ShaderProgram::Release()
{
	if (_program)
	{
		glDeleteProgram(_program);
		_program = 0;
	}
}

bool ShaderProgram::Link()
{
	_vs.lock()->Use();
	return true;
}

SharedPtr<ShaderVariation> ShaderProgram::VertexShader() const
{
	return _vs.lock();
}

SharedPtr<ShaderVariation> ShaderProgram::PixelShader() const
{
	return _ps.lock();
}



}
#endif