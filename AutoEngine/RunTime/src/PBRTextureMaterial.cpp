#include "PBRTextureMaterial.h"
#include "AtConfig.h"
#include "stb_image.h"
#include "GLWindow.h"
#include "Camera.h"
#include "LoadResource.h"
#include "BaseMesh.h"
#include "RenderManager.h"
#include "TimeManager.h"
AUTO_BEGIN
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
PBRTextureMaterial::PBRTextureMaterial()
	: pbrShader(AtConfig::shader_path + "au_pbr.auvs"
		, AtConfig::shader_path + "au_pbr_hdr_trxture.aufs")
{
}


PBRTextureMaterial::~PBRTextureMaterial()
{
}

void PBRTextureMaterial::Start()
{

	glEnable(GL_DEPTH_TEST);
	// set depth function to less than AND equal for skybox depth trick.
	glDepthFunc(GL_LEQUAL);
	// enable seamless cubemap sampling for lower mip levels in the pre-filter map.
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	pbrShader.Use();
	pbrShader.SetInt("irradianceMap", 0);
	pbrShader.SetInt("prefilterMap", 1);
	pbrShader.SetInt("brdfLUT", 2);
	pbrShader.SetInt("albedoMap", 3);
	pbrShader.SetInt("normalMap", 4);
	pbrShader.SetInt("metallicMap", 5);
	pbrShader.SetInt("roughnessMap", 6);
	pbrShader.SetInt("aoMap", 7);

	ironAlbedoMap = LocalTextureLoad("Resource/texture/pbr/gold/albedo.png");
	ironNormalMap = LocalTextureLoad("Resource/texture/pbr/gold/normal.png");
	ironMetallicMap = LocalTextureLoad("Resource/texture/pbr/gold/metallic.png");
	ironRoughnessMap = LocalTextureLoad("Resource/texture/pbr/gold/roughness.png");
	ironAOMap = LocalTextureLoad("Resource/texture/pbr/gold/ao.png");

	// gold
	goldAlbedoMap = LocalTextureLoad("Resource/texture/pbr/gold/albedo.png");
	goldNormalMap = LocalTextureLoad("Resource/texture/pbr/gold/normal.png");
	goldMetallicMap = LocalTextureLoad("Resource/texture/pbr/gold/metallic.png");
	goldRoughnessMap = LocalTextureLoad("Resource/texture/pbr/gold/roughness.png");
	goldAOMap = LocalTextureLoad("Resource/texture/pbr/gold/ao.png");

	// grass
	grassAlbedoMap = LocalTextureLoad("Resource/texture/pbr/gold/albedo.png");
	grassNormalMap = LocalTextureLoad("Resource/texture/pbr/gold/normal.png");
	grassMetallicMap = LocalTextureLoad("Resource/texture/pbr/gold/metallic.png");
	grassRoughnessMap = LocalTextureLoad("Resource/texture/pbr/gold/roughness.png");
	grassAOMap = LocalTextureLoad("Resource/texture/pbr/gold/ao.png");

	// plastic
	plasticAlbedoMap = LocalTextureLoad("Resource/texture/pbr/gold/albedo.png");
	plasticNormalMap = LocalTextureLoad("Resource/texture/pbr/gold/normal.png");
	plasticMetallicMap = LocalTextureLoad("Resource/texture/pbr/gold/metallic.png");
	plasticRoughnessMap = LocalTextureLoad("Resource/texture/pbr/gold/roughness.png");
	plasticAOMap = LocalTextureLoad("Resource/texture/pbr/gold/ao.png");

	// wall
	wallAlbedoMap = LocalTextureLoad("Resource/texture/pbr/gold/albedo.png");
	wallNormalMap = LocalTextureLoad("Resource/texture/pbr/gold/normal.png");
	wallMetallicMap = LocalTextureLoad("Resource/texture/pbr/gold/metallic.png");
	wallRoughnessMap = LocalTextureLoad("Resource/texture/pbr/gold/roughness.png");
	wallAOMap = LocalTextureLoad("Resource/texture/pbr/gold/ao.png");

}

void PBRTextureMaterial::Draw()
{
	//////////////////////////////////////////////////////////////////////////
	glm::mat4 projection = INSTANCE(RenderManager).GetCurrentCamera().GetProjectionMatrix();

	pbrShader.Use();
	pbrShader.SetMat4("projection", projection);
	glm::mat4 model;
	glm::mat4 view = INSTANCE(RenderManager).GetCurrentCamera().GetViewMatrix();
	pbrShader.SetMat4("view", view);
	pbrShader.SetVec3("camPos", INSTANCE(RenderManager).GetCurrentCamera().GetPosition());

	// bind pre-computed IBL data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(SkyBoxManager).GetSkyBox()->irradianceMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, INSTANCE(SkyBoxManager).GetSkyBox()->prefilterMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, INSTANCE(SkyBoxManager).GetSkyBox()->brdfLUTTexture);

	// rusted iron
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ironAlbedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ironNormalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, ironMetallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, ironRoughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, ironAOMap);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-5.0, 0.0, 2.0));
	pbrShader.SetMat4("model", model);
	renderSphere(&sphereVAO,&indexCount);

	// gold
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, goldAlbedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, goldNormalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, goldMetallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, goldRoughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, goldAOMap);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-3.0, 0.0, 2.0));
	pbrShader.SetMat4("model", model);
	renderSphere(&sphereVAO, &indexCount);

	// grass
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassAlbedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, grassNormalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, grassMetallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, grassRoughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, grassAOMap);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.0, 0.0, 2.0));
	pbrShader.SetMat4("model", model);
	renderSphere(&sphereVAO, &indexCount);

	// plastic
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, plasticAlbedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, plasticNormalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, plasticMetallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, plasticRoughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, plasticAOMap);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(1.0, 0.0, 2.0));
	pbrShader.SetMat4("model", model);
	renderSphere(&sphereVAO, &indexCount);

	// wall
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, wallAlbedoMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, wallNormalMap);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, wallMetallicMap);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, wallRoughnessMap);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, wallAOMap);

	model = glm::mat4();
	model = glm::translate(model, glm::vec3(3.0, 0.0, 2.0));
	pbrShader.SetMat4("model", model);
	renderSphere(&sphereVAO, &indexCount);

	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
	{
		glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(INSTANCE(TimeManager).GetCurTime() * 5.0) * 5.0, 0.0, 0.0);
		newPos = lightPositions[i];
		pbrShader.SetVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		pbrShader.SetVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

		model = glm::mat4();
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		pbrShader.SetMat4("model", model);
		renderSphere(&sphereVAO,&indexCount);
	}


}

AUTO_END