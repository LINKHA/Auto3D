#include "Graphics.h"
#include "OpenGLGather.h"
#include "GLDebug.h"
#include "GameWindow.h"
#include "GraphicsDefines.h"
AUTO_BEGIN
void Graphics::Init()
{
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
bool Graphics::BeginFrame()
{
	glEnable(GL_DEPTH);
	glEnable(GL_LESS);
	SetColorWrite(true);
	SetDepthWrite(true);
	Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL);
	return true;
}
void Graphics::EndFrame()
{
	GetSubSystem<GameWindow>()->DrawWindow();
}
void Graphics::Clear(unsigned flags, const Color & color, float depth, unsigned stencil)
{
	bool oldColorWrite = _colorWrite;
	bool oldDepthWrite = _depthWrite;

	if (flags & CLEAR_COLOR && !oldColorWrite)
		SetColorWrite(true);
	if (flags & CLEAR_DEPTH && !oldDepthWrite)
		SetDepthWrite(true);
	if (flags & CLEAR_STENCIL && _stencilWriteMask != MATH_MAX_UNSIGNED)
		glStencilMask(MATH_MAX_UNSIGNED);

	unsigned glFlags = 0;
	if (flags & CLEAR_COLOR)
	{
		glFlags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color.r, color.g, color.b, color.a);
	}
	if (flags & CLEAR_DEPTH)
	{
		glFlags |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & CLEAR_STENCIL)
	{
		glFlags |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	glClear(glFlags);
}

void Graphics::SetColorWrite(bool enable)
{
	if (enable != _colorWrite)
	{
		if (enable)
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		else
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		_colorWrite = enable;
	}
}
void Graphics::SetDepthWrite(bool enable)
{
	if (enable != _depthWrite)
	{
		glDepthMask(enable ? GL_TRUE : GL_FALSE);
		_depthWrite = enable;
	}
}
AUTO_END