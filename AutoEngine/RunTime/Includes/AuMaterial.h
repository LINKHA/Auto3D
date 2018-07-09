#ifndef AU_MATERIAL_H_
#define AU_MATERIAL_H_

#include "Auto.h"
#include "Math/AUMath.h"
#include "ObjectDefines.h"
#include "Image.h"
USING_MATH
AUTO_BEGIN
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
AUTO_END
#endif // AU_MATERIAL_H_