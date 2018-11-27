#include "GLTextureEnable.h"
#include "DebugNew.h"


namespace Auto3D {
GLTextureEnable::GLTextureEnable()
	: _useStencil(false)
	, _useDepth(true)
	, _useBlend(false)
{
}


GLTextureEnable::~GLTextureEnable()
{
}
void GLTextureEnable::GLApply()
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
	if (_useBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
void GLTextureEnable::GLOriginal()
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
	if (_useBlend)
	{
		glDisable(GL_BLEND);

	}
}

void GLTextureEnable::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	_sfail = sfail;
	_dpfail = dpfail;
	_dppass = dppass;
}
void GLTextureEnable::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	_func = func;
	_ref = ref;
	_mask = mask;
}
void GLTextureEnable::StencilMask(GLuint mask)
{
	_mas = mask;
}
void GLTextureEnable::DepthFunc(GLenum func)
{
	_depthfunc = func;
}
}