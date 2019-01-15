#include "OffScreen.h"
#include "AutoOGL.h"
#include "Rect.h"
#include "Graphics.h"
#include "VertexData.h"
#include "BaseMesh.h"
#include "ResourceSystem.h"

#include "NewDef.h"

namespace Auto3D {

OffScreen::OffScreen(SharedPtr<Ambient> ambient)
	: Super(ambient)
	, _samplingPointCount(1)
	, _isAllowMsaa(false)
	, _isAllowLateEffect(false)
{

	auto cach = GetSubSystem<ResourceSystem>();
	
	shader = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_offscreen.glsl"));
	shader->Create();

	shaderBlur = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_offscreen_blur.glsl"));
	shaderBlur->Create();
	
	shaderEdgeDetection = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_offscreen_edge_detection.glsl"));
	shaderEdgeDetection->Create();

	shaderGrayscale = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_offscreen_grayscale.glsl"));
	shaderGrayscale->Create();

	shaderInversion = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_offscreen_inversion.glsl"));
	shaderInversion->Create();

	shaderSharpen = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_offscreen_sharpen.glsl"));
	shaderSharpen->Create();

	hdrShader = MakeShared<ShaderVariation>(cach->GetResource<Shader>("shader/au_hdr.glsl"));
	hdrShader->Create();

	_shader = shader.get();
}

OffScreen::~OffScreen()
{
}
void OffScreen::bindHdr()
{
	auto cach = GetSubSystem<ResourceSystem>();
	//Temp !!! Hdr
	if (_isAllowHDR)
		_shader = hdrShader.get();


	glGenFramebuffers(1, &_framebuffer);
	// create floating point color buffer

	glGenTextures(1, &_screenTexture);
	glBindTexture(GL_TEXTURE_2D, _screenTexture);

	RectInt t = GetSubSystem<Graphics>()->GetWindowRectInt();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, t.width, t.height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create depth buffer (renderbuffer)

	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, t.width, t.height);
	// attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _screenTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ErrorString("Framebuffer not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void OffScreen::bindMsaaAndPostpro()
{
	if (_isAllowHDR)
		_shader = hdrShader.get();

	GLint value;
	glGetIntegerv(GL_MAX_SAMPLES, &value);
	_samplingPointCount = clamp(_samplingPointCount, 1, static_cast<int>(value));

	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();

	////////////////////////////////////////////////////////////////////////
	//Bind MSAA frame buffers
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



	//////////////////////////////////////////////////////////////////////////
	//Bind screen texture
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
void OffScreen::RenderReady()
{
	
#pragma warning
	if (_isAllowHDR)
		bindHdr();
	else
		bindMsaaAndPostpro();


	_shader->Use();
	_shader->SetInt("hdrBuffer", 0);
}


void OffScreen::RenderStart()
{
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void OffScreen::RenderEnd()
{
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();


	glBindFramebuffer(GL_READ_FRAMEBUFFER, _framebuffer);
	if(_isAllowMsaa)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	if(_isAllowLateEffect || _isAllowHDR)
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _intermediateFBO);

	glBlitFramebuffer(0, 0, rect.width, rect.height, 0, 0, rect.width, rect.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//////////////////////////////////////////////////////////////////////////
	if (_isAllowLateEffect || _isAllowHDR)
	{
		
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glDisable(GL_DEPTH_TEST);

		_shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _screenTexture);
		
		
		//Temp !!! hdr
		if (_isAllowHDR)
		{
			_shader->SetInt("hdr", true);
			_shader->SetFloat("exposure", 1.0f);
		}
		//End

		renderQuad(&_quadVAO, &_quadVBO);
	}
	
}

void OffScreen::SetEffect(PostProcessingMode mode)
{
	switch (mode)
	{
	case POST_DEFAULT:
		_shader = shader.get();
		break;
	case POST_BULR:
		_shader = shaderBlur.get();
		break;
	case POST_EDGE_DETECTION:
		_shader = shaderEdgeDetection.get();
		break;
	case POST_GRAYSCALE:
		_shader = shaderGrayscale.get();
		break;
	case POST_INVERSION:
		_shader = shaderInversion.get();
		break;
	case POST_SHARPEN:
		_shader = shaderSharpen.get();
		break;
	default:
		_shader = shader.get();
		break;
	}
}
void OffScreen::SetEffect(ShaderVariation* shader)
{
	_shader = shader;
}
void OffScreen::AllowMSAA(bool enable, int pointNum)
{
	 _isAllowMsaa = enable;
	 _samplingPointCount = pointNum; 
}

}
