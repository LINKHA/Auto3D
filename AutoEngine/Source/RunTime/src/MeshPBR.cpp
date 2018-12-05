#include "MeshPBR.h"
#include "Configs.h"
#include "BaseMesh.h"
#include "HDRSkyBox.h"
#include "Renderer.h"
#include "Time.h"
#include "Transform.h"
#include "Light.h"
#include "NewDef.h"

namespace Auto3D {

MeshPBR::MeshPBR(Ambient* ambient)
	: Super(ambient)
	, _shaderTexture(shader_path + "au_pbr.auvs"
		, shader_path + "au_pbr_hdr.aufs")
	, _shaderNoTexture(shader_path + "au_pbr.auvs"
		, shader_path + "au_pbr.aufs")
	, _metallic(1.0f)
	, _roughness(0.02f)
{
	_albedo.Set(1.0f, 1.0f, 1.0f);
}


MeshPBR::~MeshPBR()
{
	UnloadOpaque(this);
}

void MeshPBR::Start()
{
	if (SkyBoxManager::Instance().GetEnable())
	{
		_shader = _shaderTexture;
	}
	else
	{
		_shader = _shaderNoTexture;
	}
	_shader.Use();
	if (SkyBoxManager::Instance().GetEnable())
	{
		_shader.SetInt("irradianceMap", 0);
		_shader.SetInt("prefilterMap", 1);
		_shader.SetInt("brdfLUT", 2);
	}
	_shader.SetVec3("albedo", _albedo);
	_shader.SetFloat("ao", 1.0f);
	RegisterOpaque(this);
}

void MeshPBR::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	

	_shader.Use();
	_shader.SetMat4("projection", projection);
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	_shader.SetMat4("view", view);
	_shader.SetVec3("camPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	if (SkyBoxManager::Instance().GetEnable())
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManager::Instance().GetSkyBox()->irradianceMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxManager::Instance().GetSkyBox()->prefilterMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, SkyBoxManager::Instance().GetSkyBox()->brdfLUTTexture);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	//light
	VECTOR<Light*> lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int dir = 0;
	for (VECTOR<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		_shader.SetVec3("lightPositions[" + KhSTL::ToString(dir) + "]", (*it)->GetNode().GetPosition());
		_shader.SetVec3("lightColors[" + KhSTL::ToString(dir) + "]", (*it)->GetColorToVec());
		dir++;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	//model
	glm::mat4 modelMat;

	if (GetNodePtr())		//if gameObject not empty
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	_shader.SetFloat("metallic", _metallic);
	_shader.SetFloat("roughness", _roughness);
	_shader.SetMat4("model", modelMat);

	renderSphere(&_vao, &_indexCount);

}


}
