#include "Graphics.h"
#include "OpenGLGather.h"
#include "GLDebug.h"
#include "GameWindow.h"

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
	Clear();
	return true;
}
void Graphics::EndFrame()
{
	GetSubSystem<GameWindow>()->DrawWindow();
}
void Graphics::Clear()
{
	unsigned glFlags = 0;
	glFlags |= GL_COLOR_BUFFER_BIT;
	glFlags |= GL_DEPTH_BUFFER_BIT;
	glFlags |= GL_STENCIL_BUFFER_BIT;
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