#include "MeshShadow.h"
#include "Renderer.h"
#include "Light.h"
#include "VertexData.h"
#include "ShadowTest.h"
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
	glm::mat4 model;
	switch (k)
	{
	case 0:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(10.0f, 0.5f, 10.0f));
		shadowShader.SetMat4("model", model);
		_model->Draw(shadowShader);
		break;
	case 1:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shadowShader.SetMat4("model", model);
		_model->Draw(shadowShader);
		break;
	case 2:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		shadowShader.SetMat4("model", model);
		_model->Draw(shadowShader);
		break;
	case 3:
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		shadowShader.SetMat4("model", model);
		_model->Draw(shadowShader);
		break;
	}

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
		glm::mat4 model;
		switch (k)
		{
		case 0:
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
			model = glm::scale(model, glm::vec3(10.0f, 0.5f, 10.0f));
			_shader.SetMat4("model", model);
			_model->Draw(_shader);
			break;
		case 1:
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
			model = glm::scale(model, glm::vec3(0.5f));
			_shader.SetMat4("model", model);
			_model->Draw(_shader);
			break;
		case 2:
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
			model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
			model = glm::scale(model, glm::vec3(0.25));
			_shader.SetMat4("model", model);
			_model->Draw(_shader);
			break;
		case 3:
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
			model = glm::scale(model, glm::vec3(0.5f));
			_shader.SetMat4("model", model);
			_model->Draw(_shader);
			break;
		}
	}
}

}
