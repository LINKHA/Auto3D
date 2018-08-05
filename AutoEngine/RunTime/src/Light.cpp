#include "Light.h"
#include "LogAssert.h"
namespace Auto3D {
/////////////////////////////////////////////////////////////////////////////////////////////
//ShadowRenderAssist
/////////////////////////////////////////////////////////////////////////////////////////////
ShadowRenderAssist::ShadowRenderAssist(Ambient* ambient)
	: Super(ambient)
	, _shadowWidth(1024)
	, _shadowHeight(1024)
{

}
ShadowRenderAssist::~ShadowRenderAssist()
{

}
void ShadowRenderAssist::BindDirDepathMap()
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

void ShadowRenderAssist::BindPointDepathMap()
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

/////////////////////////////////////////////////////////////////////////////////////////////
//Light
/////////////////////////////////////////////////////////////////////////////////////////////
Light::Light(Ambient* ambi)
	: Super(ambi)
	, _shadowAssist(nullptr)
	, _farPlane(100)
{
	AddToManager();
}
void Light::Update()
{
	Assert(GetGameObjectPtr());

	GameObject* gameObject = GetGameObjectPtr();
	float nearPlane = 1.0f;
	_lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, _farPlane);
	_lightViewMatrix = glm::lookAt(gameObject->GetComponent(Transform).GetPosition().ToGLM(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	_lightSpaceMatrix = _lightProjectionMatrix * _lightViewMatrix;
}
void Light::SetShadowType(ShadowType type)
{
	_shadowType = type;
	if (_shadowAssist || (type == SHADOW_TYPE_NO_SHADOW))
		return;
	_shadowAssist = new ShadowRenderAssist(_ambient);
}
Light::~Light()
{}
glm::mat4 & Light::GetLightSpaceMatrix()
{
	return _lightSpaceMatrix;
}
glm::mat4 & Light::GetLightProjectionMatrix()
{
	return _lightProjectionMatrix;
}
glm::mat4 & Light::GetLightViewMatrix()
{
	return _lightViewMatrix;
}
void Light::AddToManager()
{
	INSTANCE(LightManager).AddLight(this);
}
void Light::RemoveFromManager()
{
	INSTANCE(LightManager).RemoveLight(this);
}

}