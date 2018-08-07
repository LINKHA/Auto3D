#include "MeshShadow.h"
#include "Renderer.h"
#include "Light.h"
#include "VertexData.h"
#include "Camera.h"
#include "Configs.h"
#include "LoadResource.h"

namespace Auto3D {

MeshShadow::MeshShadow(Ambient* ambient)
	: RenderComponent(ambient)
	, _shader(Shader(shader_path + "au_shadow_mapping.auvs"
		, shader_path + "au_shadow_mapping.aufs"))
{
	RegisterShadow(this);
	RegisterOpaque(this);
}
MeshShadow::MeshShadow(Ambient* ambient,int i)
	: RenderComponent(ambient)
	, _shader(Shader(shader_path + "au_shadow_mapping.auvs"
		, shader_path + "au_shadow_mapping.aufs"))
{
	k = i;
	RegisterShadow(this);
	RegisterOpaque(this);
}

MeshShadow::~MeshShadow()
{
	UnloadShadow(this);
	UnloadOpaque(this);
}
void MeshShadow::DrawReady()
{
	_model = LocalModelLoad("../Resource/object/base/Cube.FBX");

	_woodTexture = LocalTextureLoad("../Resource/texture/wood.jpg");
	_shader.Use();
	_shader.SetInt("diffuseTexture", 0);
	_shader.SetInt("shadowMap", 1);
}

void MeshShadow::DrawShadow()
{
	Shader& shadowShader = GetSubSystem<Renderer>()->GetShadowRenderer()->GetDepthMapShader();
	glm::mat4 modelMat;

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	shadowShader.SetMat4("model", modelMat);
	_model->Draw(shadowShader);
}
void MeshShadow::Draw()
{
	Camera* camera = GetSubSystem<Renderer>()->GetCurrentCameraPtr();
	glm::vec3 lightPos;
	glm::mat4 lightSpaceMatrix;
	//!!! Temp use one light,and must need light
	#pragma warning
	_VECTOR(Light*)& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	//!!!Temp
	Assert(lights.size() != 0);
	for (_VECTOR(Light*)::iterator it = lights.begin(); it != lights.end(); it++)
	{
		lightPos = (*it)->GetLightPosition();
		lightSpaceMatrix = (*it)->GetLightSpaceMatrix();

		unsigned depthMap = (*it)->GetShadowAssist()->GetDepthMap();
		_shader.Use();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		_shader.SetMat4("projection", projection);
		_shader.SetMat4("view", view);
		// set light uniforms
		_shader.SetVec3("viewPos", camera->GetPosition());
		_shader.SetVec3("lightPos", lightPos);
		_shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glm::mat4 modelMat;

		if (GetGameObjectPtr())		//if gameObject not empty
			modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
		else
			modelMat = Matrix4x4::identity;
		_shader.SetMat4("model", modelMat);

		_model->Draw(_shader);

	}
}

}
