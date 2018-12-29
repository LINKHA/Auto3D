#include "Material.h"
#include "ResourceSystem.h"
#include "NewDef.h"
namespace Auto3D {
Material::Material(SharedPtr<Ambient> ambi)
	:Super(ambi)
	,isTexture(false)
{
	color = Color(1.0f, 1.0f, 1.0f);
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
	diffuseMap = GetSubSystem<ResourceSystem>()->TextureLoad(path);
}
}


