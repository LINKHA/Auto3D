#include "MeshPBRTexture.h"
#include "BaseMesh.h"
#include "HDRSkyBox.h"
#include "Renderer.h"
#include "Time.h"
#include "Transform.h"
#include "Light.h"
#include "ResourceSystem.h"
#include "Config.h"
#include "NewDef.h"

namespace Auto3D {

MeshPBRTexture::MeshPBRTexture(SharedPtr<Ambient> ambient)
	: Super(ambient)
{
	auto cach = GetSubSystem<ResourceSystem>();
	auto vs = cach->GetResource<Shader>("shader/au_pbr.glvs");
	auto fs = cach->GetResource<Shader>("shader/au_pbr_hdr_trxture.glfs");
	_shaderTexture = MakeShared<ShaderVariation>(vs,fs);
	_shaderTexture->Create();

	auto nvs = cach->GetResource<Shader>("shader/au_pbr.glvs");
	auto nfs = cach->GetResource<Shader>("shader/au_pbr_hdr_trxture.glfs");
	_shaderNoTexture = MakeShared<ShaderVariation>(nvs, nfs);
	_shaderNoTexture->Create();
}


MeshPBRTexture::~MeshPBRTexture()
{
	
}

void MeshPBRTexture::Destory()
{
	UnloadOpaque(SharedFromThis());
}

void MeshPBRTexture::Start()
{
	if (Singleton<SkyBoxManager>::Instance().GetEnable())
	{
		_shader = _shaderTexture;
	}
	else
	{
		_shader = _shaderNoTexture;
	}
	_shader->Use();

	_shader->SetInt("albedoMap", 0);
	_shader->SetInt("normalMap", 1);
	_shader->SetInt("metallicMap", 2);
	_shader->SetInt("roughnessMap", 3);
	_shader->SetInt("aoMap", 4);
	
	if (Singleton<SkyBoxManager>::Instance().GetEnable())
	{
		
		_shader->SetInt("irradianceMap", 5);
		_shader->SetInt("prefilterMap", 6);
		_shader->SetInt("brdfLUT", 7);
	}

	STRING dir = system_content_dictionary;

	_albedoMap = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/pbr/gold/albedo.png").CStr());
	_normalMap = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/pbr/gold/normal.png").CStr());
	_metallicMap = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/pbr/gold/metallic.png").CStr());
	_roughnessMap = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/pbr/gold/roughness.png").CStr());
	_aoMap = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/pbr/gold/ao.png").CStr());

	RegisterOpaque(SharedFromThis());
}

void MeshPBRTexture::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();

	_shader->Use();
	_shader->SetMat4("projection", projection);
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	_shader->SetMat4("view", view);
	_shader->SetVec3("camPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _albedoMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _normalMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _metallicMap);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _roughnessMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _aoMap);
	// bind pre-computed IBL data
	if (Singleton<SkyBoxManager>::Instance().GetEnable())
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Singleton<SkyBoxManager>::Instance().GetSkyBox()->irradianceMap);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_CUBE_MAP, Singleton<SkyBoxManager>::Instance().GetSkyBox()->prefilterMap);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, Singleton<SkyBoxManager>::Instance().GetSkyBox()->brdfLUTTexture);
		
	}
	else
	{
		
	}

	

	/////////////////////////////////////////////////////////////////////////////////////////////
	//light
	VECTOR<SharedPtr<Light> > lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int lightNum = 0;
	for (VECTOR<SharedPtr<Light> >::iterator it = lights.begin(); it != lights.end(); it++)
	{
		_shader->SetVec3("lightPositions[" + KhSTL::ToString(lightNum) + "]", (*it)->GetNode()->GetPosition());
		_shader->SetVec3("lightColors[" + KhSTL::ToString(lightNum) + "]", (*it)->GetColorToVec());
		lightNum++;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	//model
	glm::mat4 modelMat;

	if (GetNode())		//if gameObject not empty
		modelMat = GetNode()->GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	_shader->SetMat4("model", modelMat);

	renderSphere(&_vao, &_indexCount);

}


}
