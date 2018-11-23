#include "Material.h"
#include "ResourceSystem.h"
namespace Auto3D {
Material::Material(Ambient* ambi)
	:Super(ambi)
	,isTexture(false)
{
	color = Color(1.0f, 1.0f, 1.0f);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.5f, 0.5f, 0.5f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	shininess = 32.0f;
}
Material::Material(Ambient* ambi,char * path)
	:Super(ambi)
	,isTexture(true)
{
	//diffuseMap = LocalTextureLoad(path);
	diffuseMap = GetSubSystem<ResourceSystem>()->TextureLoad(path);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.5f, 0.5f, 0.5f);
	specular = Vector3(1.0f, 1.0f, 1.0f);
	shininess = 32.0f;
}
Material::~Material()
{}

void Material::SetImage(char * path)
{
	isTexture = true;
	//diffuseMap = LocalTextureLoad(path);
	diffuseMap = GetSubSystem<ResourceSystem>()->TextureLoad(path);
}
}


