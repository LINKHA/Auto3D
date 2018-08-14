#pragma once
#include "RenderComponent.h"
#include "Shader.h"
#include "Math/MathBase.h"
namespace Auto3D {
class MeshPBR : public RenderComponent
{
	REGISTER_DERIVED_CLASS(MeshPBR, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(MeshPBR);
public:
	explicit MeshPBR(Ambient* ambient);
	void Start()override;
	void Draw()override;
	/**
	* @brief : Set metallic clamp 0~1
	*/
	void SetMetallic(float value) { _metallic = clamp01(value); }
	/**
	* @brief : Set roughress clamp 0~1
	*/
	void SetRoughness(float value) { _roughness = clamp01(value); }
private:
	Shader _shader;
	unsigned _vao;
	unsigned _indexCount;

	float _metallic;
	float _roughness;
};

}