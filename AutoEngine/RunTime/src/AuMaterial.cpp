#include "AuMaterial.h"
#include "LoadResource.h"

AUTO_BEGIN
Material::Material()
	:isTexture(false)
{
	color = Color(1.0f, 1.0f, 1.0f);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.5f, 0.5f, 0.5f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	shininess = 32.0f;
}
Material::Material(char * path)
	:isTexture(true)
{
	diffuseMap = LocalTextureLoad(path);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.5f, 0.5f, 0.5f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	shininess = 32.0f;
}

void Material::SetImage(char * path)
{
	isTexture = true;
	diffuseMap = LocalTextureLoad(path);
}
AUTO_END


