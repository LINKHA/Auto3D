#pragma once
#include "RenderComponent.h"
#include "Mesh.h"

namespace Auto3D {
class DeferredShading : public RenderComponent, public EnableSharedFromThis<DeferredShading>
{
public:
	explicit DeferredShading(SharedPtr<Ambient> ambient);
	~DeferredShading();
	void Destory()override;
	void Start()override;
	void Draw()override;
private:
	SharedPtr<ShaderVariation> m_shaderGeometryPass;
	SharedPtr<ShaderVariation> m_shaderLightingPass;
	SharedPtr<ShaderVariation> m_shaderLightBox;

	SharedPtr<Mesh> nanosuit;

	VECTOR<glm::vec3> objectPositions;
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	
	unsigned int rboDepth;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

};
}
