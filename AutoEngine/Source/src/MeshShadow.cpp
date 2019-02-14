#include "MeshShadow.h"
#include "Renderer.h"
#include "tLight.h"
#include "VertexData.h"
#include "tCamera.h"
#include "ResourceSystem.h"
#include "Mesh.h"
#include "Config.h"
#include "NewDef.h"

namespace Auto3D {

MeshShadow::MeshShadow(SharedPtr<Ambient> ambient)
	: RenderComponent(ambient)
{
	auto shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_shadow_mapping.glsl");
	_shader = MakeShared<ShaderVariation>(shader);
	_shader->Create();
	
}

MeshShadow::~MeshShadow()
{
}

void MeshShadow::Destory()
{
	UnloadShadow(SharedFromThis(MeshShadow));
	UnloadOpaque(SharedFromThis(MeshShadow));
}

void MeshShadow::Init()
{
	RegisterShadow(SharedFromThis(MeshShadow));
	RegisterOpaque(SharedFromThis(MeshShadow));
}

void MeshShadow::DrawReady()
{
	STRING dir = system_content_dictionary;
	_mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");
	_woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/wood.jpg").CStr());


	
	_shader->Use();
	_shader->SetInt("diffuseTexture", 0);
	_shader->SetInt("shadowMap", 1);
}

void MeshShadow::DrawShadow()
{
	auto shadowShader = GetSubSystem<Renderer>()->GetShadowRenderer()->GetDepthMapShader();
	glm::mat4 modelMat;

	if (GetNode())		//if gameObject not empty
		modelMat = GetNode()->GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	shadowShader->SetMat4("model", modelMat);
	_mesh->DrawMesh(shadowShader);
}
void MeshShadow::Draw()
{
	SharedPtr<tCamera> camera = GetSubSystem<Renderer>()->GetCurrentCameraPtr();
	glm::vec3 lightPos;
	glm::mat4 lightSpaceMatrix;
	//!!! Temp use one light,and must need light
	#pragma warning
	VECTOR<SharedPtr<tLight> >& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	//!!!Temp
	Assert(lights.size() != 0);

	for (VECTOR<SharedPtr<tLight> >::iterator it = lights.begin(); it != lights.end(); it++)
	{
		lightPos = (*it)->GetLightPosition();
		lightSpaceMatrix = (*it)->GetLightSpaceMatrix();

		unsigned depthMap = (*it)->GetShadowAssist()->GetDepthMap();
		_shader->Use();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		_shader->SetMat4("projection", projection);
		_shader->SetMat4("view", view);
		// set light uniforms
		_shader->SetVec3("viewPos", camera->GetPosition());
		_shader->SetVec3("lightPos", lightPos);
		_shader->SetMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glm::mat4 modelMat;

		if (GetNode())		//if gameObject not empty
			modelMat = GetNode()->GetComponent<Transform>()->GetTransformMat();
		else
			modelMat = Matrix4x4::identity;
		_shader->SetMat4("model", modelMat);
		_mesh->DrawMesh(_shader);

	}
}

}
