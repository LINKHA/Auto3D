#pragma once
#include "Material.h"

namespace Auto3D
{

class JSONValue;

class PBRMaterial : public Material
{
	REGISTER_OBJECT_CLASS(PBRMaterial, Material)
public:
	/// Constructor
	PBRMaterial();
	/// Destructor
	~PBRMaterial();

	/// Register object factory.
	static void RegisterObject();
	/// Finalize material loading in the main thread. Return true on success.
	bool EndLoad();

	/// Load json with PBR
	void LoadJSON(const JSONValue& source);
private:
	Vector3F _albedo;

	float _metallic;

	float _roughness;

	float _ao;
};

}