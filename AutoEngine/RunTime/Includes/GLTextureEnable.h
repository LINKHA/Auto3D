#ifndef GL_ENABLE_H_
#define GL_ENABLE_H_
#include "OpenGLGather.h"
#include "Auto.h"
AUTO_BEGIN
TOOL_CLASS GLTextureEnable
{
public:
	GLTextureEnable();
	virtual ~GLTextureEnable();
	
	void EnableStencil(bool enable) { m_useStencil = enable; }
	void EnableDepth(bool enable) { m_useDepth = enable; }
	void EnableBlend(bool enable) { m_useBlend = enable; }

	void GLApply();
	void GLOriginal();


	void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void StencilFunc(GLenum func, GLint ref, GLuint mask);
	void StencilMask(GLuint mask);
	void DepthFunc(GLenum func);
protected:
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
	bool m_useStencil;
	bool m_useDepth;
	bool m_useBlend;
};
AUTO_END
#endif // !GL_ENABLE_H_
