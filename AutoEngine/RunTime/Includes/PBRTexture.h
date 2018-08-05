#pragma once
#include "RenderComponent.h"
#include "Shader.h"
#include "GLGather.h"

namespace Auto3D {

class PBRTexture : public RenderComponent
{
public:
	explicit PBRTexture(Ambient* ambient);
	~PBRTexture();
	void Start()override;
	void Draw()override;

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

}