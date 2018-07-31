#pragma once

#include "Auto.h"
#include "Math/Math.h"
#include "ObjectDefines.h"
#include "Image.h"

namespace Auto3D {
struct Material
{
	Material();
	Material(char * path);
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