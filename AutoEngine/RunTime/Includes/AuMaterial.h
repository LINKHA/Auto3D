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
public:
	Material(const Vector3& ambi = Vector3(1.0f, 1.0f, 1.0f),const Vector3& diff = Vector3(1.0f, 1.0f, 1.0f), const Vector3& specr = Vector3(1.0f, 1.0f, 1.0f), float shin = 32.0f);
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	float shininess;
	Image * image;

	void SetImage(char* path);
	
	
};
AUTO_END
#endif // AU_MATERIAL_H_