#include "FrameBuffersScreen.h"
#include "OpenGLGather.h"
#include "Monitors.h"
#include "AtConfig.h"
#include "GameWindow.h"
#include "VertexData.h"
AUTO_BEGIN
Shader shader;
Shader shaderBlur;
Shader shaderEdgeDetection;
Shader shaderGrayscale;
Shader shaderInversion;
Shader shaderSharpen;

FrameBuffersScreen::FrameBuffersScreen(Ambient* ambient)
	:Super(ambient)
	,_enable(false)
//	,_shader(Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",
//		AtConfig::shader_path + "au_framebuffers_screen.aufs"))
{
	_shader = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",
		AtConfig::shader_path + "au_framebuffers_screen.aufs");
	shader = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen.aufs");
	shaderBlur = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_blur.aufs");
	shaderEdgeDetection = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_edge_detection.aufs");
	shaderGrayscale = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_grayscale.aufs");
	shaderInversion = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_inversion.aufs");
	shaderSharpen = Shader(AtConfig::shader_path + "au_framebuffers_screen.auvs",AtConfig::shader_path + "au_framebuffers_screen_sharpen.aufs");

}
FrameBuffersScreen::~FrameBuffersScreen()
{
	glDeleteVertexArrays(1, &_quadVAO);
	glDeleteBuffers(1, &_quadVBO);
}



void FrameBuffersScreen::Start()
{
	glGenVertexArrays(1, &_quadVAO);
	glGenBuffers(1, &_quadVBO);
	glBindVertexArray(_quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), &quad_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	_shader.Use();
	_shader.SetInt("screenTexture", 0);

	RectInt screenSize = GetSubSystem<GameWindow>()->GetWindowRectInt();
	

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glGenTextures(1, &_textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.width, screenSize.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _textureColorbuffer, 0);
	
	
	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenSize.width, screenSize.height); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
																							
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffersScreen::DrawStart()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glEnable(GL_DEPTH_TEST);
}
void FrameBuffersScreen::DrawEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); 
	
	glBindVertexArray(_quadVAO);
	glBindTexture(GL_TEXTURE_2D, _textureColorbuffer);
	_shader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void FrameBuffersScreen::SetEffect(BuffersMode mode)
{
	switch (mode)
	{
	case kDefault:
		_shader = shader;
		break;
	case kBlur:
		_shader = shaderBlur;
		break;
	case kEdge_detection:
		_shader = shaderEdgeDetection;
		break;
	case kGrayscale:
		_shader = shaderGrayscale;
		break;
	case kInversion:
		_shader = shaderInversion;
		break;
	case kSharpen:
		_shader = shaderSharpen;
		break;
	default:
		_shader = shader;
		break;
	}
}
void FrameBuffersScreen::SetEffect(const Shader& shader)
{
	_shader = shader;
}
AUTO_END