#include "AuMaterial.h"
#include "LoadResource.h"
AUTO_BEGIN
Material::Material()
{
	color = Color(1.0f, 1.0f, 1.0f);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.5f, 0.5f, 0.5f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	shininess = 32.0f;
}

void Material::SetImage(char* path)
{
	image = LocalImageLoad(path);
}
AUTO_END


