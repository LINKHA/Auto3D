#pragma once
#include "RenderComponent.h"
#include "_Shader.h"
#include "Mesh.h"

namespace Auto3D {
class DeferredShading : public RenderComponent
{
public:
	explicit DeferredShading(Ambient* ambient);
	~DeferredShading();
	void Start()override;
	void Draw()override;
private:
	_Shader m_shaderGeometryPass;
	_Shader m_shaderLightingPass;
	_Shader m_shaderLightBox;

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
