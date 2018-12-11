#pragma once
#include "RenderComponent.h"
#include "_Shader.h"

namespace Auto3D {

class MeshPBRTexture : public RenderComponent
{
	REGISTER_DERIVED_CLASS(MeshPBRTexture, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(MeshPBRTexture);
public:
	explicit MeshPBRTexture(Ambient* ambient);
	void Start()override;
	void Draw()override;
private:
	_Shader _shader;
	unsigned _vao;
	unsigned _indexCount;

	_Shader _shaderNoTexture;
	_Shader _shaderTexture;

	unsigned int _albedoMap;
	unsigned int _normalMap;
	unsigned int _metallicMap;
	unsigned int _roughnessMap;
	unsigned int _aoMap;

};

}