#include "Texture2D.h"
#include "RenderManager.h"
#include "VertexData.h"
AUTO_BEGIN

Texture2D::Texture2D() 
	: useStencil(false)
	, useDepth(true)
	, useBlend(false)
{}
Texture2D::~Texture2D()
{

}


void Texture2D::Start()
{}

void Texture2D::Draw(Camera * cam)
{}



void Texture2D::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail;
	m_dpfail = dpfail;
	m_dppass = dppass;
}
void Texture2D::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func;
	m_ref = ref;
	m_mask = mask;
}
void Texture2D::StencilMask(GLuint mask)
{
	m_mas = mask;
}
void Texture2D::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}

AUTO_END