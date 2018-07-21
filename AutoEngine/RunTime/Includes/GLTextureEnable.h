#pragma once
#include "OpenGLGather.h"
#include "Auto.h"
AUTO_BEGIN
TOOL_CLASS GLTextureEnable
{
public:
	GLTextureEnable();
	virtual ~GLTextureEnable();
	
	void EnableStencil(bool enable) { _useStencil = enable; }
	void EnableDepth(bool enable) { _useDepth = enable; }
	void EnableBlend(bool enable) { _useBlend = enable; }

	void GLApply();
	void GLOriginal();


	void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void StencilFunc(GLenum func, GLint ref, GLuint mask);
	void StencilMask(GLuint mask);
	void DepthFunc(GLenum func);
protected:
	GLenum _sfail; GLenum _dpfail; GLenum _dppass;
	GLenum _func; GLint _ref; GLuint _mask;
	GLuint _mas;
	GLenum _depthfunc;
	bool _useStencil;
	bool _useDepth;
	bool _useBlend;
};
AUTO_END

