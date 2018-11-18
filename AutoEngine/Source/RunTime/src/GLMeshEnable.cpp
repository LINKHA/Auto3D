#include "GLMeshEnable.h"


namespace Auto3D {
GLMeshEnable::GLMeshEnable()
	: _useStencil(false)
	, _useDepth(true)
	, _useFullFace(true)
{
}


GLMeshEnable::~GLMeshEnable()
{
}
void GLMeshEnable::GLApply()
{
	if (_useDepth)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(_depthfunc);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}

	if (_useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		if (_sfail)
			glStencilOp(_sfail, _dpfail, _dppass);
		else
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if (_func)
			glStencilFunc(_func, _ref, _mask);
		else
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glStencilMask(_mas);

	}

	if (_useFullFace)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}
void GLMeshEnable::GLOriginal()
{
	if (_useStencil)
	{
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
	}
	if (!_useDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDepthFunc(GL_LESS);
	if (_useFullFace)
	{
		glCullFace(GL_FRONT);
		glDisable(GL_CULL_FACE);
	}
}
void GLMeshEnable::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	_sfail = sfail;
	_dpfail = dpfail;
	_dppass = dppass;
}
void GLMeshEnable::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	_func = func;
	_ref = ref;
	_mask = mask;
}
void GLMeshEnable::StencilMask(GLuint mask)
{
	_mas = mask;
}
void GLMeshEnable::DepthFunc(GLenum func)
{
	_depthfunc = func;
}
}