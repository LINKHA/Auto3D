#include "MeshPBRTexture.h"
#include "Configs.h"
#include "LoadResource.h"
#include "BaseMesh.h"
#include "HDRSkyBox.h"
#include "Renderer.h"
#include "Time.h"
#include "Transform.h"
#include "Light.h"
namespace Auto3D {

MeshPBRTexture::MeshPBRTexture(Ambient* ambient)
	: Super(ambient)
	, _shader(shader_path + "au_pbr.auvs"
		, shader_path + "au_pbr_hdr_trxture.aufs")
{
	
}


MeshPBRTexture::~MeshPBRTexture()
{
	UnloadOpaque(this);
}

void MeshPBRTexture::Start()
{
	_shader.Use();
	_shader.SetInt("irradianceMap", 0);
	_shader.SetInt("prefilterMap", 1);
	_shader.SetInt("brdfLUT", 2);
	_shader.SetInt("albedoMap", 3);
	_shader.SetInt("normalMap", 4);
	_shader.SetInt("metallicMap", 5);
	_shader.SetInt("roughnessMap", 6);
	_shader.SetInt("aoMap", 7);

	_albedoMap = LocalTextureLoad("../Resource/texture/pbr/gold/albedo.png");
	_normalMap = LocalTextureLoad("../Resource/texture/pbr/gold/normal.png");
	_metallicMap = LocalTextureLoad("../Resource/texture/pbr/gold/metallic.png");
	_roughnessMap = LocalTextureLoad("../Resource/texture/pbr/gold/roughness.png");
	_aoMap = LocalTextureLoad("../Resource/texture/pbr/gold/ao.png");


	RegisterOpaque(this);
}

void MeshPBRTexture::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();

	_shader.Use();
	_shader.SetMat4("projection", projection);
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	_shader.SetMat4("view", view);
	_shader.SetVec3("camPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	// bind pre-computed IBL data
	if (INSTANCE(SkyBoxManager).GetEnable())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(SkyBoxManager).GetSkyBox()->irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(SkyBoxManager).GetSkyBox()->prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, INSTANCE(SkyBoxManager).GetSkyBox()->brdfLUTTexture);
	}

	// rusted iron
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _albedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _normalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _metallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, _roughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, _aoMap);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//light
	_VECTOR(Light*) lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int dir = 0;
	for (_VECTOR(Light*)::iterator it = lights.begin(); it != lights.end(); it++)
	{
		_shader.SetVec3("lightPositions[" + std::to_string(dir) + "]", (*it)->GetGameObject().GetPosition());
		_shader.SetVec3("lightColors[" + std::to_string(dir) + "]", (*it)->GetColorToVec());
		dir++;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	//model
	glm::mat4 modelMat;

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	_shader.SetMat4("model", modelMat);

	renderSphere(&_vao, &_indexCount);

}


}
