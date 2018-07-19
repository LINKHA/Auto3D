#pragma once
#include "RenderComponent.h"
#include "Shader.h"
AUTO_BEGIN
class SkyBoxReflectMesh : public RenderComponent
{
public:
	SkyBoxReflectMesh(); 
	SkyBoxReflectMesh(const Shader& shader);
	~SkyBoxReflectMesh();
	void Start()override;
	void Draw()override;
private:
	Shader m_shader;
	unsigned int cubeVAO, cubeVBO;
};

AUTO_END