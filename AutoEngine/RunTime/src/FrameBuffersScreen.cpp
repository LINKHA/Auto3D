#include "FrameBuffersScreen.h"
#include "OpenGLGather.h"
#include "Monitors.h"
#include "AtConfig.h"
#include "GLWindow.h"
#include "VertexData.h"
AUTO_BEGIN
Shader shader;
Shader shaderBlur;
Shader shaderEdgeDetection;
Shader shaderGrayscale;
Shader shaderInversion;
Shader shaderSharpen;


SINGLETON_INSTANCE(FrameBuffersScreen);
FrameBuffersScreen::FrameBuffersScreen()
	:m_enable(false)
	,m_shader(Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",
		AtConfig::shader_path + "au_framebuffers_screen.aufs"))
{
	shader = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen.aufs");
	shaderBlur = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_blur.aufs");
	shaderEdgeDetection = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_edge_detection.aufs");
	shaderGrayscale = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_grayscale.aufs");
	shaderInversion = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_inversion.aufs");
	shaderSharpen = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_sharpen.aufs");

}
FrameBuffersScreen::~FrameBuffersScreen()
{
	glDeleteVertexArrays(1, &m_quadVAO);
	glDeleteBuffers(1, &m_quadVBO);
}



void FrameBuffersScreen::Start()
{
	glGenVertexArrays(1, &m_quadVAO);
	glGenBuffers(1, &m_quadVBO);
	glBindVertexArray(m_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	m_shader.Use();
	m_shader.SetInt("screenTexture", 0);

	RectInt screenSize = INSTANCE(GLWindow).GetWindowRectInt();
	

	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	glGenTextures(1, &m_textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.width, screenSize.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureColorbuffer, 0);
	
	
	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenSize.width, screenSize.height); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
																							
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffersScreen::DrawStart()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
	glEnable(GL_DEPTH_TEST);
}
void FrameBuffersScreen::DrawEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); 
	
	glBindVertexArray(m_quadVAO);
	glBindTexture(GL_TEXTURE_2D, m_textureColorbuffer);
	m_shader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void FrameBuffersScreen::SetEffect(BuffersMode mode)
{
	switch (mode)
	{
	case Default:
		m_shader = shader;
		break;
	case Blur:
		m_shader = shaderBlur;
		break;
	case Edge_detection:
		m_shader = shaderEdgeDetection;
		break;
	case Grayscale:
		m_shader = shaderGrayscale;
		break;
	case Inversion:
		m_shader = shaderInversion;
		break;
	case Sharpen:
		m_shader = shaderSharpen;
		break;
	default:
		m_shader = shader;
		break;
	}
}
void FrameBuffersScreen::SetEffect(const Shader& shader)
{
	m_shader = shader;
}
AUTO_END