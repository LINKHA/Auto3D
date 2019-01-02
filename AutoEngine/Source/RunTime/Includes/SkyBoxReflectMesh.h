#pragma once
#include "RenderComponent.h"
#include "ShaderVariation.h"
namespace Auto3D {
class SkyBoxReflectMesh : public RenderComponent, public EnableSharedFromThis<SkyBoxReflectMesh>
{
public:
	explicit SkyBoxReflectMesh(SharedPtr<Ambient> ambient); 
	SkyBoxReflectMesh(SharedPtr<Ambient> ambient, SharedPtr<Shader> shader);
	~SkyBoxReflectMesh();
	void Destory()override;
	void Start()override;
	void Draw()override;
private:
	SharedPtr<ShaderVariation> _shader;
	unsigned int cubeVAO, cubeVBO;
};

}