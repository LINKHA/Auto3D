#pragma once
#include "RenderComponent.h"
#include "_Shader.h"
namespace Auto3D {
class SkyBoxReflectMesh : public RenderComponent
{
public:
	explicit SkyBoxReflectMesh(Ambient* ambient); 
	SkyBoxReflectMesh(Ambient* ambient,const _Shader& shader);
	~SkyBoxReflectMesh();
	void Start()override;
	void Draw()override;
private:
	_Shader m_shader;
	unsigned int cubeVAO, cubeVBO;
};

}