#pragma once
#include "Auto.h"
#include "Math/Math.h"
#include "ComponentSetting.h"
namespace Auto3D {
class Material : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(Material, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(Material);
public:
	explicit Material(Ambient* ambient);
	Material(Ambient* ambient,char * path);
	unsigned int diffuseMap;
	Color color;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;
	bool isTexture;

	void SettImage(char * path);
	
};
}