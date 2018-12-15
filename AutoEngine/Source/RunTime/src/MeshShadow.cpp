#include "MeshShadow.h"
#include "Renderer.h"
#include "Light.h"
#include "VertexData.h"
#include "Camera.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "Mesh.h"
#include "NewDef.h"

namespace Auto3D {

MeshShadow::MeshShadow(Ambient* ambient)
	: RenderComponent(ambient)
	, _tshader(_Shader(shader_path + "au_shadow_mapping.auvs"
		, shader_path + "au_shadow_mapping.aufs"))
{
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
	Mesh* tmp = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("../Resource/object/base/Cube.3DS");
	_mesh = SharedPtr<Mesh>(tmp);
	_woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/wood.jpg");


	
	_tshader.Use();
	_tshader.SetInt("diffuseTexture", 0);
	_tshader.SetInt("shadowMap", 1);
}

void MeshShadow::DrawShadow()
{
	_Shader& shadowShader = GetSubSystem<Renderer>()->GetShadowRenderer()->GetDepthMapShader();
	glm::mat4 modelMat;

	if (GetNodePtr())		//if gameObject not empty
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	shadowShader.SetMat4("model", modelMat);
	_mesh->DrawMesh(shadowShader);
}
void MeshShadow::Draw()
{
	Camera* camera = GetSubSystem<Renderer>()->GetCurrentCameraPtr();
	glm::vec3 lightPos;
	glm::mat4 lightSpaceMatrix;
	//!!! Temp use one light,and must need light
	#pragma warning
	VECTOR<Light*>& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	//!!!Temp
	Assert(lights.size() != 0);
	for (VECTOR<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		lightPos = (*it)->GetLightPosition();
		lightSpaceMatrix = (*it)->GetLightSpaceMatrix();

		unsigned depthMap = (*it)->GetShadowAssist()->GetDepthMap();
		_tshader.Use();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		_tshader.SetMat4("projection", projection);
		_tshader.SetMat4("view", view);
		// set light uniforms
		_tshader.SetVec3("viewPos", camera->GetPosition());
		_tshader.SetVec3("lightPos", lightPos);
		_tshader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glm::mat4 modelMat;

		if (GetNodePtr())		//if gameObject not empty
			modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
		else
			modelMat = Matrix4x4::identity;
		_tshader.SetMat4("model", modelMat);

		_mesh->DrawMesh(_tshader);

	}
}

}
