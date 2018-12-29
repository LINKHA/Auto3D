#pragma once
#include "RenderComponent.h"
#include "ShaderVariation.h"
#include "Math/Math.h"


namespace Auto3D {


class MeshPBR : public RenderComponent
{
	REGISTER_OBJECT_CLASS(MeshPBR, RenderComponent)
public:
	explicit MeshPBR(SharedPtr<Ambient> ambient);
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
	void SetAlbedo(float r, float g, float b) { _albedo.Set(r, g, b); }
	void SetAlbedo(const Vector3& vec) { _albedo = vec; }
private:
	SharedPtr<ShaderVariation> _shader;

	unsigned _vao;
	unsigned _indexCount;

	float _metallic;
	float _roughness;
	Vector3 _albedo;

	SharedPtr<ShaderVariation> _shaderTexture;
	SharedPtr<ShaderVariation> _shaderNoTexture;
};

}