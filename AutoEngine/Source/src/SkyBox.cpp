#include "SkyBox.h"
#include "Graphics.h"
#include "tCamera.h"
#include "VertexData.h"
#include "Renderer.h"
#include "ResourceSystem.h"
#include "Renderer.h"
#include "ShaderVariation.h"
#include "Config.h"
#include "NewDef.h"


namespace Auto3D {


SkyBox::SkyBox(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


SkyBox::~SkyBox()
{
	UnloadOpaque(SharedFromThis(SkyBox));
}

void SkyBox::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<SkyBox>(SCENE_ATTACH);
}

void SkyBox::Start()
{
	Singleton<SkyManager>::Instance().AddSkyBox(this);
	auto shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_skybox.glsl");
	_shader = MakeShared<ShaderVariation>(shader);
	_shader->Create();
	glGenBuffers(1, &_skyboxVBO);

	glBindBuffer(GL_ARRAY_BUFFER, _skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	STRING dir = system_content_dictionary;

	VECTOR<STRING> faces
	{
	dir + "skybox/arrakisday_ft.tga",
	dir + "skybox/arrakisday_bk.tga",
	dir + "skybox/arrakisday_up.tga",
	dir + "skybox/arrakisday_dn.tga",
	dir + "skybox/arrakisday_rt.tga",
	dir + "skybox/arrakisday_lf.tga"
	};
	//_cubemapTexture = LoadCubemap(faces);
	_cubemapTexture = GetSubSystem<ResourceSystem>()->CubemapLoad(faces);
	RegisterOpaque(SharedFromThis(SkyBox));
}
void SkyBox::Draw()
{
	glm::mat4 viewMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	RectInt rect = GetSubSystem<Graphics>()->GetWindowRectInt();
	glm::mat4 projectionMat = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glDepthFunc(GL_LEQUAL);  
	_shader->Use();
	viewMat = glm::mat4(glm::mat3(GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix()));
	_shader->SetMat4("view", viewMat);
	_shader->SetMat4("projection", projectionMat);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);
}

}