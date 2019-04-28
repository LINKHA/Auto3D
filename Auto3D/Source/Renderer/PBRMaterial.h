#pragma once
#include "Material.h"

namespace Auto3D
{

class PBRMaterial :public Material
{
	REGISTER_OBJECT_CLASS(PBRMaterial, Material)
public:
	PBRMaterial() {}
	~PBRMaterial() {}

private:
	Vector3F _albedo;

	float _metallic;

	float _roughness;

	float _ao;
};

}