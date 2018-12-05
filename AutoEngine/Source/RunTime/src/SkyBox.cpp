#include "SkyBox.h"
#include "Graphics.h"
#include "Camera.h"
#include "VertexData.h"
#include "Renderer.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "Renderer.h"
#include "NewDef.h"


namespace Auto3D {


SkyBox::SkyBox(Ambient* ambient)
	:Super(ambient)
{
}


SkyBox::~SkyBox()
{
	UnloadOpaque(this);
}
void SkyBox::Start()
{
	SkyManager::Instance().AddSkyBox(this);
	_shader = Shader(shader_path + "au_skybox.auvs", shader_path + "au_skybox.aufs");
	glGenVertexArrays(1, &_skyboxVAO);
	glGenBuffers(1, &_skyboxVBO);
	glBindVertexArray(_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	VECTOR<STRING> faces
	{
	"../Resource/skybox/arrakisday_ft.tga",
	"../Resource/skybox/arrakisday_bk.tga",
	"../Resource/skybox/arrakisday_up.tga",
	"../Resource/skybox/arrakisday_dn.tga",
	"../Resource/skybox/arrakisday_rt.tga",
	"../Resource/skybox/arrakisday_lf.tga"
	};
	//_cubemapTexture = LoadCubemap(faces);
	_cubemapTexture = GetSubSystem<ResourceSystem>()->CubemapLoad(faces);
	RegisterOpaque(this);
}
void SkyBox::Draw()
{
	glm::mat4 viewMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	glm::mat4 projectionMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glDepthFunc(GL_LEQUAL);  
	_shader.Use();
	viewMat = glm::mat4(glm::mat3(GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix()));
	_shader.SetMat4("view", viewMat);
	_shader.SetMat4("projection", projectionMat);
	glBindVertexArray(_skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

}