#include "IBLMaterial.h"

#include "../Engine/ModuleManager.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Texture.h"
#include "../Renderer/SkyBox.h"
#include "../Scene/Scene.h"
#include "../RegisteredBox/RegisteredBox.h"
#include "../Debug/Log.h"

#include "../Debug/DebugNew.h"


namespace Auto3D
{

IBLMaterial::IBLMaterial():
	_iblCubeMap(nullptr)
{

}

IBLMaterial::~IBLMaterial()
{

}

void IBLMaterial::RegisterObject()
{
	RegisterFactory<IBLMaterial>();
}
bool IBLMaterial::EndLoad()
{
	bool flag = false;
	flag = Super::EndLoad();
	auto scene = ModuleManager::Get().RegisteredBoxModule()->GetActiveScene();
	if (scene)
		flag &= SetupIBL(scene->GetSkyBox());
	return flag;
}


bool IBLMaterial::SetupIBL(SkyBox* skybox)
{
	if (skybox == NULL || skybox->GetMaterial(0)->_textures[0] == nullptr)
	{
		ErrorString("Failed to create ibl perhaps because skybox was not created");
		return false;
	}
	
	if (_iblCubeMap != nullptr && _iblCubeMap == skybox->GetMaterial(0)->_textures[0])
		return true;

	_iblCubeMap = skybox->GetMaterial(0)->_textures[0];

	SetTexture(5, skybox->GetIrradianceMap());
	SetTexture(6, skybox->GetPrefilterMap());
	SetTexture(7, skybox->GetBrdfLUT());

	return true;
}

}