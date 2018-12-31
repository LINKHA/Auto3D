#include "Light.h"
#include "Renderer.h"
#include "NewDef.h"

namespace Auto3D {
/////////////////////////////////////////////////////////////////////////////////////////////
//ShadowRenderAssist
/////////////////////////////////////////////////////////////////////////////////////////////
ShadowRenderAssist::ShadowRenderAssist(SharedPtr<Ambient> ambient,LightType type)
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
	if (_type == LightType::Directional)
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
	else if (_type == LightType::Point || _type == LightType::Spot)
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
	if (_type == LightType::Directional)
		return _depthDirMapFBO;
	else if (_type == LightType::Point || _type == LightType::Spot)
		return _depthPointMapFBO;
	else
	{
		ErrorString("Fail get depth map fbo");
		return NULL;
	}
}

unsigned ShadowRenderAssist::GetDepthMap()
{
	if (_type == LightType::Directional)
		return _depthDirMap;
	else if (_type == LightType::Point || _type == LightType::Spot)
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
Light::Light(SharedPtr<Ambient> ambi)
	: Super(ambi)
	, _shadowAssist(nullptr)
	, _farPlane(25.0f)
	, _nearPlane(0.01f)
{
	SetType(LightType::Directional);
	AddToManager();
	SetShadowType(ShadowType::Soft);
}

Light::~Light()
{}

void Light::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Light>(SCENE_ATTACH);
}

void Light::Update()
{
	Assert(GetNode());

	float nearPlane = 1.0f;
	_lightProjectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, _farPlane);
	_lightViewMatrix = glm::lookAt(GetNode()->GetComponent<Transform>()->GetPosition().ToGLM(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	_lightSpaceMatrix = _lightProjectionMatrix * _lightViewMatrix;
}

void Light::SetType(LightType type)
{
	if (type == LightType::Directional)
	{
		_type = type;
		_color.Set(1.0f, 1.0f, 1.0f);
		direction.Set(-0.5f, -0.5f, -0.0f);
		ambient.Set(0.3f, 0.3f, 0.3f);
		diffuse.Set(1.0f, 1.0f, 1.0f);
		specular.Set(0.7f, 0.7f, 0.7f);
	}
	else if (type == LightType::Point)
	{
		_type = type;
		_color.Set(1.0f, 1.0f, 1.0f);
		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;
		ambient.Set(0.3f, 0.3f, 0.3f);
		diffuse.Set(1.0f, 1.0f, 1.0f);
		specular.Set(0.7f, 0.7f, 0.7f);
	}
	else if (type == LightType::Spot)
	{
		_type = type;
		_color.Set(1.0f, 1.0f, 1.0f);
		direction.Set(-0.5f, -0.5f, -0.0f);
		cutOff = glm::cos(glm::radians(12.5f));
		outerCutOff = glm::cos(glm::radians(15.0f));
		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;
		ambient.Set(0.3f, 0.3f, 0.3f);
		diffuse.Set(1.0f, 1.0f, 1.0f);
		specular.Set(0.7f, 0.7f, 0.7f);
	}
	else
	{
		ErrorString("Fail to set light type");
	}
}

void Light::SetShadowType(ShadowType type)
{
	_shadowType = type;
	if (_shadowAssist || (type == ShadowType::NoShadow))
		return;
	_shadowAssist = SharedPtr<ShadowRenderAssist>(new ShadowRenderAssist(_ambient,GetType()));
}

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
	Assert(GetNode());
	return GetNode()->GetComponent<Transform>()->GetPosition().ToGLM();
}
void Light::AddToManager()
{
	GetSubSystem<Renderer>()->GetLightContainer()->AddLight(this);
}
void Light::RemoveFromManager()
{
	GetSubSystem<Renderer>()->GetLightContainer()->RemoveLight(this);
}

}