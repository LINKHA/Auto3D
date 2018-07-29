#pragma once
#include "RenderComponent.h"
#include "Shader.h"
AUTO_BEGIN
class SkyBoxReflectMesh : public RenderComponent
{
public:
	SkyBoxReflectMesh(Ambient* ambient); 
	SkyBoxReflectMesh(Ambient* ambient,const Shader& shader);
	~SkyBoxReflectMesh();
	void Start()override;
	void Draw()override;
private:
	Shader m_shader;
	unsigned int cubeVAO, cubeVBO;
};

AUTO_END