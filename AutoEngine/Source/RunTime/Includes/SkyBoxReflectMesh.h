#pragma once
#include "RenderComponent.h"
#include "ShaderVariation.h"
namespace Auto3D {
class SkyBoxReflectMesh : public RenderComponent
{
public:
	explicit SkyBoxReflectMesh(Ambient* ambient); 
	SkyBoxReflectMesh(Ambient* ambient,Shader* shader);
	~SkyBoxReflectMesh();
	void Start()override;
	void Draw()override;
private:
	SharedPtr<ShaderVariation> _shader;
	unsigned int cubeVAO, cubeVBO;
};

}