#pragma once
#include "RenderComponent.h"
#include "Shader.h"
#include "Model.h"

namespace Auto3D {
class DeferredShading : public RenderComponent
{
public:
	explicit DeferredShading(Ambient* ambient);
	~DeferredShading();
	void Start()override;
	void Draw()override;
private:
	Shader m_shaderGeometryPass;
	Shader m_shaderLightingPass;
	Shader m_shaderLightBox;

	sharedPtr<Model> nanosuit;

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
