#pragma once
#include "Gameobject.h"
#include "Shader.h"
#include "OpenGLGather.h"

AUTO_BEGIN

class PBRTexture : public Component
{
public:
	PBRTexture();
	~PBRTexture();
	void Start()override;
	void Draw(Camera * camera = nullptr)override;

private:
	Shader m_shader;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;

	unsigned int albedo;
	unsigned int normal;
	unsigned int metallic;
	unsigned int roughness;
	unsigned int ao;
};

AUTO_END