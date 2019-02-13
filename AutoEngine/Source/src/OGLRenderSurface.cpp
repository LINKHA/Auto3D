#include "RenderSurface.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "Texture.h"

namespace Auto3D {


RenderSurface::RenderSurface(SharedPtr<Texture> parentTexture)
	: _parentTexture(parentTexture)
	, _target(GL_TEXTURE_2D)
	, _renderBuffer(0)
{

}

void RenderSurface::Release()
{

}

bool RenderSurface::CreateRenderBuffer(unsigned width, unsigned height, unsigned format, int multiSample)
{
	auto graphics = _parentTexture->GetGraphics().lock();
	if (!graphics)
		return false;
	Release();

	glGenRenderbuffers(1, &_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _renderBuffer);
	if (multiSample > 1)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, multiSample, format, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	return true;
}



}
#endif