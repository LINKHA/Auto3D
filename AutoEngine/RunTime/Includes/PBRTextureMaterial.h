#pragma once
#include "RenderComponent.h"
#include "Shader.h"
namespace Auto3D {
class PBRTextureMaterial : public RenderComponent
{
public:
	explicit PBRTextureMaterial(Ambient* ambient);
	~PBRTextureMaterial();
	void Start()override;
	void Draw()override;
private:
	Shader pbrShader;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;

	unsigned int ironAlbedoMap;
	unsigned int ironNormalMap;
	unsigned int ironMetallicMap;
	unsigned int ironRoughnessMap;
	unsigned int ironAOMap;

	// gold
	unsigned int goldAlbedoMap;
	unsigned int goldNormalMap;
	unsigned int goldMetallicMap;
	unsigned int goldRoughnessMap;
	unsigned int goldAOMap;

	// grass
	unsigned int grassAlbedoMap;
	unsigned int grassNormalMap;
	unsigned int grassMetallicMap;
	unsigned int grassRoughnessMap;
	unsigned int grassAOMap;

	// plastic
	unsigned int plasticAlbedoMap;
	unsigned int plasticNormalMap;
	unsigned int plasticMetallicMap;
	unsigned int plasticRoughnessMap;
	unsigned int plasticAOMap;

	// wall
	unsigned int wallAlbedoMap;
	unsigned int wallNormalMap;
	unsigned int wallMetallicMap;
	unsigned int wallRoughnessMap;
	unsigned int wallAOMap;
};

}