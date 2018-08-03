#include "MSAA.h"
#include "OpenGLGather.h"
#include "Math/Rect.h"
#include "Graphics.h"
#include "VertexData.h"
#include "Configs.h"
namespace Auto3D {

MSAA::MSAA(Ambient* ambient, int pointNum)
	: Super(ambient)
	, _shader(shader_path + "au_anti_aliasing_offscreen.auvs"
		, shader_path + "au_anti_aliasing_offscreen.aufs")
{
	//Clamp sampling point count(1~8)
	_samplingPointCount = clamp(pointNum, 1, 8);

	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	if (_samplingPointCount < 0)
	{
		WarningString("Fail to antialiasing with sampling point count subter 0");
		return;
	}
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glGenTextures(1, &_textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samplingPointCount, GL_RGB, rect.width, rect.height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, _textureColorBufferMultiSampled, 0);

	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, _samplingPointCount, GL_DEPTH24_STENCIL8, rect.width, rect.height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &_intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _intermediateFBO);

	glGenTextures(1, &_screenTexture);
	glBindTexture(GL_TEXTURE_2D, _screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, rect.width, rect.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _screenTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Intermediate framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
MSAA::~MSAA()
{
}

void MSAA::RenderStart()
{
	if (_samplingPointCount < 0)
	{
		WarningString("Fail to antialiasing with sampling point count subter 0");
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}
void MSAA::RenderEnd()
{
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	if (_samplingPointCount < 0)
	{
		WarningString("Fail to antialiasing with sampling point count subter 0");
		return;
	}
	glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediateFBO);
	glBlitFramebuffer(0, 0, rect.width, rect.height, 0, 0, rect.width, rect.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	_shader.Use();
	glBindVertexArray(_quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
}