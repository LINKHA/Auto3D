#pragma once
#include "RenderComponent.h"
#include "ShaderVariation.h"

namespace Auto3D {

class MeshPBRTexture : public RenderComponent
{
	REGISTER_OBJECT_CLASS(MeshPBRTexture, RenderComponent)
public:
	explicit MeshPBRTexture(SharedPtr<Ambient> ambient);
	void Start()override;
	void Draw()override;
private:
	SharedPtr<ShaderVariation> _shader;
	unsigned _vao;
	unsigned _indexCount;

	SharedPtr<ShaderVariation> _shaderNoTexture;
	SharedPtr<ShaderVariation> _shaderTexture;

	unsigned int _albedoMap;
	unsigned int _normalMap;
	unsigned int _metallicMap;
	unsigned int _roughnessMap;
	unsigned int _aoMap;

};

}