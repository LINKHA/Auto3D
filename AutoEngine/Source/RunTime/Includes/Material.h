#pragma once
#include "Auto.h"
#include "Math/Math.h"
#include "Object.h"

namespace Auto3D {
class Material : public Object
{
	REGISTER_OBJECT_CLASS(Material, Object)
public:
	explicit Material(SharedPtr<Ambient> ambient);
	unsigned int diffuseMap;
	Color color;
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;
	bool isTexture;

	void SetImage(char * path);
};
}