#include "MSAA.h"
#include "OpenGLGather.h"
#include "Math/RectT.h"
#include "GLWindow.h"
USING_MATH
AUTO_BEGIN
SINGLETON_INSTANCE(MSAA);
float ms_quadVertices[] = {  
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

RectInt ms_rect = INSTANCE(GLWindow).GetWindowRectInt();

MSAA::MSAA()
	:m_Shader(AtConfig::shader_path + "au_anti_aliasing_offscreen.auvs"
		, AtConfig::shader_path + "au_anti_aliasing_offscreen.aufs")
{
}
MSAA::~MSAA()
{
}

void MSAA::Start(int samplingPointCount)
{
	m_SamplingPointCount = samplingPointCount;
	if (samplingPointCount <= 0)
	{
		WarningString("Fail to antialiasing with sampling point count subter 0");
		return;
	}
	glGenVertexArrays(1, &ms_quadVAO);
	glGenBuffers(1, &ms_quadVBO);
	glBindVertexArray(ms_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ms_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ms_quadVertices), &ms_quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &ms_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, ms_framebuffer);

	glGenTextures(1, &ms_textureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ms_textureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samplingPointCount, GL_RGB, ms_rect.width, ms_rect.height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, ms_textureColorBufferMultiSampled, 0);

	glGenRenderbuffers(1, &ms_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, ms_rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplingPointCount, GL_DEPTH24_STENCIL8, ms_rect.width , ms_rect.height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ms_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &ms_intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ms_intermediateFBO);

	glGenTextures(1, &ms_screenTexture);
	glBindTexture(GL_TEXTURE_2D, ms_screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ms_rect.width, ms_rect.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ms_screenTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Intermediate framebuffer is not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void MSAA::UpdateStart()
{
	if (m_SamplingPointCount <= 0)
	{
		WarningString("Fail to antialiasing with sampling point count subter 0");
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, ms_framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}
void MSAA::UpdateEnd()
{
	if (m_SamplingPointCount <= 0)
	{
		WarningString("Fail to antialiasing with sampling point count subter 0");
		return;
	}
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ms_framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ms_intermediateFBO);
	glBlitFramebuffer(0, 0, ms_rect.width, ms_rect.height, 0, 0, ms_rect.width, ms_rect.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	m_Shader.Use();
	glBindVertexArray(ms_quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ms_screenTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);

}
AUTO_END