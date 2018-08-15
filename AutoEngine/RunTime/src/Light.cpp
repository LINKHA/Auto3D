#include "Light.h"
#include "LogAssert.h"
#include "Renderer.h"
namespace Auto3D {
/////////////////////////////////////////////////////////////////////////////////////////////
//ShadowRenderAssist
/////////////////////////////////////////////////////////////////////////////////////////////
ShadowRenderAssist::ShadowRenderAssist(Ambient* ambient,LightType type)
	: Super(ambient)
	, _shadowWidth(1024)
	, _shadowHeight(1024)
	, _type(type)
{

}
ShadowRenderAssist::~ShadowRenderAssist()
{

}
void ShadowRenderAssist::BindDepathMap()
{
	if (_type == kDirectional)
	{
		glGenFramebuffers(1, &_depthDirMapFBO);
		// create depth texture

		glGenTextures(1, &_depthDirMap);
		glBindTexture(GL_TEXTURE_2D, _depthDirMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _shadowWidth, _shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, _depthDirMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthDirMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else if (_type == kPoint || _type == kSpot)
	{
		glGenFramebuffers(1, &_depthPointMapFBO);
		// create depth cubemap texture
		glGenTextures(1, &_depthPointmap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _depthPointmap);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, _shadowWidth, _shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// attach depth texture as FBO's depth buffer
		glBindFramebuffer(GL_FRAMEBUFFER, _depthPointMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthPointmap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		ErrorString("Fail bind depath map");
	}
	
}
unsigned ShadowRenderAssist::GetDepthMapFBO()
{
	if (_type == kDirectional)
		return _depthDirMapFBO;
	else if (_type == kPoint || _type == kSpot)
		return _depthPointMapFBO;
	else
	{
		ErrorString("Fail get depth map fbo");
		return NULL;
	}
}

unsigned ShadowRenderAssist::GetDepthMap()
{
	if (_type == kDirectional)
		return _depthDirMap;
	else if (_type == kPoint || _type == kSpot)
		return _depthPointmap;
	else
	{
		ErrorString("Fail get depth map");
		return NULL;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
//Light
/////////////////////////////////////////////////////////////////////////////////////////////
Light::Light(Ambient* ambi,LightType type)
	: Super(ambi)
	, _type(type)
	, _shadowAssist(nullptr)
	, _farPlane(25.0f)
	, _nearPlane(0.01f)
{
	AddToManager();
	SetShadowType(SHADOW_TYPE_SOFT);
}
void Light::Update()
{
	Assert(GetGameObjectPtr());

	float nearPlane = 1.0f;
	_lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, _farPlane);
	_lightViewMatrix = glm::lookAt(GetGameObjectPtr()->GetComponent(Transform).GetPosition().ToGLM(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	_lightSpaceMatrix = _lightProjectionMatrix * _lightViewMatrix;
}
void Light::SetShadowType(ShadowType type)
{
	_shadowType = type;
	if (_shadowAssist || (type == SHADOW_TYPE_NO_SHADOW))
		return;
	_shadowAssist = new ShadowRenderAssist(_ambient,GetType());
}
Light::~Light()
{}
glm::mat4& Light::GetLightSpaceMatrix()
{
	return _lightSpaceMatrix;
}
glm::mat4& Light::GetLightProjectionMatrix()
{
	return _lightProjectionMatrix;
}
glm::mat4& Light::GetLightViewMatrix()
{
	return _lightViewMatrix;
}
glm::vec3 Light::GetLightPosition()
{
	Assert(GetGameObjectPtr());
	return GetGameObject().GetComponent(Transform).GetPosition().ToGLM();
}
void Light::AddToManager()
{
	//INSTANCE(LightManager).AddLight(this);
	GetSubSystem<Renderer>()->GetLightContainer()->AddLight(this);
}
void Light::RemoveFromManager()
{
	//INSTANCE(LightManager).RemoveLight(this);
	GetSubSystem<Renderer>()->GetLightContainer()->RemoveLight(this);
}

}