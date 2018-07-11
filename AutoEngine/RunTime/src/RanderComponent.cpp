#include "RanderComponent.h"

AUTO_BEGIN

RanderComponent::RanderComponent()
	: useStencil(false)
	, useDepth(true)
	, useBlend(false)
{
}


RanderComponent::~RanderComponent()
{
}

void RanderComponent::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail;
	m_dpfail = dpfail;
	m_dppass = dppass;
}
void RanderComponent::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func;
	m_ref = ref;
	m_mask = mask;
}
void RanderComponent::StencilMask(GLuint mask)
{
	m_mas = mask;
}
void RanderComponent::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}
AUTO_END