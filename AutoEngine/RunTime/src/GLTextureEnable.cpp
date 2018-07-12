#include "GLTextureEnable.h"


AUTO_BEGIN
GLTextureEnable::GLTextureEnable()
	: m_useStencil(false)
	, m_useDepth(true)
	, m_useBlend(false)
{
}


GLTextureEnable::~GLTextureEnable()
{
}
void GLTextureEnable::GLApply()
{
	if (m_useDepth)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(m_depthfunc);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (m_useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		if (m_sfail)
			glStencilOp(m_sfail, m_dpfail, m_dppass);
		else
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if (m_func)
			glStencilFunc(m_func, m_ref, m_mask);
		else
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

		glStencilMask(m_mas);

	}
	if (m_useBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
void GLTextureEnable::GLOriginal()
{
	if (m_useStencil)
	{
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
	}
	if (!m_useDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDepthFunc(GL_LESS);
	if (m_useBlend)
	{
		glDisable(GL_BLEND);

	}
}

void GLTextureEnable::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail;
	m_dpfail = dpfail;
	m_dppass = dppass;
}
void GLTextureEnable::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func;
	m_ref = ref;
	m_mask = mask;
}
void GLTextureEnable::StencilMask(GLuint mask)
{
	m_mas = mask;
}
void GLTextureEnable::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}
AUTO_END