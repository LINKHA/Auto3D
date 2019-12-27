#include "IBLMaterial.h"

#include "Core/Modules/ModuleManager.h"
#include "Graphics/Graphics.h"
#include "Graphics/Texture.h"
#include "Engine/Components/SkyBox.h"
#include "Scene/World.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Debug/Log.h"

#include "Debug/DebugNew.h"


namespace Auto3D
{

// Font cpp fail,Initializes the global constructor so apply here
// Apply in Material CPP
//REGISTER_CLASS
//{
//	REGISTER_CALSS_FACTORY_IMP(AIBLMaterial)
//	.constructor<>()
//	;
//}

AIBLMaterial::AIBLMaterial():
	_iblCubeMap(nullptr)
{

}

AIBLMaterial::~AIBLMaterial()
{

}

bool AIBLMaterial::EndLoad()
{
	bool flag = false;
	flag = Super::EndLoad();
	auto scene = GModuleManager::Get().RegisteredBoxModule()->GetActiveWorld();
	if (scene)
		flag &= SetupIBL(scene->GetSkyBox());
	return flag;
}


bool AIBLMaterial::SetupIBL(ASkyBox* skybox)
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