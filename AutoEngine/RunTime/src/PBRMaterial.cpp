#include "PBRMaterial.h"
#include "Configs.h"
#include "stb_image.h"
#include "LoadResource.h"
#include "BaseMesh.h"
#include "HDRSkyBox.h"
#include "Renderer.h"
#include "Time.h"
namespace Auto3D {
glm::vec3 lightPositions[] = {
	glm::vec3(-10.0f,  10.0f, 10.0f),
	glm::vec3(10.0f,  10.0f, 10.0f),
	glm::vec3(-10.0f, -10.0f, 10.0f),
	glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f),
	glm::vec3(300.0f, 300.0f, 300.0f)
};
PBRMaterial::PBRMaterial(Ambient* ambient)
	: Component(ambient)
	, pbrShader(shader_path + "au_pbr.auvs"
		, shader_path + "au_pbr_hdr.aufs")
{
}


PBRMaterial::~PBRMaterial()
{
}

void PBRMaterial::Start()
{
	pbrShader.Use();
	pbrShader.SetInt("irradianceMap", 0);
	pbrShader.SetInt("prefilterMap", 1);
	pbrShader.SetInt("brdfLUT", 2);
	pbrShader.SetVec3("albedo", 1.0f, 1.0f, 1.0f);
	pbrShader.SetFloat("ao", 1.0f);
}

void PBRMaterial::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	
	pbrShader.Use();
	pbrShader.SetMat4("projection", projection);
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	pbrShader.SetMat4("view", view);
	pbrShader.SetVec3("camPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(SkyBoxManager).GetSkyBox()->irradianceMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(SkyBoxManager).GetSkyBox()->prefilterMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, INSTANCE(SkyBoxManager).GetSkyBox()->brdfLUTTexture);


	glm::mat4 model;

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	for (int row = 0; row < nrRows; ++row)
	{
		pbrShader.SetFloat("metallic", (float)row / (float)nrRows);
		for (int col = 0; col < nrColumns; ++col)
		{
			// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			pbrShader.SetFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

			model = glm::mat4();
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				-2.0f
			));
			pbrShader.SetMat4("model", model);
			renderSphere(&sphereVAO, &indexCount);
		}
	}


	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
	{
		glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(GetSubSystem<Time>()->GetCurTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = lightPositions[i];
		pbrShader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		pbrShader.SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

		model = glm::mat4();
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		pbrShader.SetMat4("model", model);
		renderSphere(&sphereVAO, &indexCount);
	}

}

}
