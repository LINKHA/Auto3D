#include "AuMaterial.h"
#include "LoadResource.h"
AUTO_BEGIN
Material::Material(const Vector3& ambi, const Vector3& diff, const Vector3& specr, float shin)
	: ambient(ambi)
	, diffuse(diff)
	, specular(specr)
	, shininess(shin)
	
{
}
void Material::SetImage(char* path)
{
	image = LocalImageLoad(path);
}
AUTO_END


