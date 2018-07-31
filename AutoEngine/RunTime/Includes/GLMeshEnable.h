#pragma once
#include "OpenGLGather.h"
#include "Auto.h"
namespace Auto3D {
TOOL_CLASS GLMeshEnable
{
public:
	GLMeshEnable();
	virtual ~GLMeshEnable();

	void EnableStencil(bool enable) { _useStencil = enable; }
	void EnableDepth(bool enable) { _useDepth = enable; }
	void EnableCullFace(bool enable) { _useFullFace = enable; }

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
	bool _useFullFace;
};
}

