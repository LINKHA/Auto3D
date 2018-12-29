#pragma once
#include "RenderComponent.h"
#include "ShaderVariation.h"
namespace Auto3D {
class SkyBoxReflectMesh : public RenderComponent
{
public:
	explicit SkyBoxReflectMesh(SharedPtr<Ambient> ambient); 
	SkyBoxReflectMesh(SharedPtr<Ambient> ambient, SharedPtr<Shader> shader);
	~SkyBoxReflectMesh();
	void Start()override;
	void Draw()override;
private:
	SharedPtr<ShaderVariation> _shader;
	unsigned int cubeVAO, cubeVBO;
};

}