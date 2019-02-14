#include "MeshShadowPoint.h"
#include "Renderer.h"
#include "tLight.h"
#include "VertexData.h"
#include "tCamera.h"
#include "ResourceSystem.h"
#include "Mesh.h"
#include "ShaderVariation.h"
#include "Config.h"
#include "NewDef.h"

namespace Auto3D {

MeshShadowPoint::MeshShadowPoint(SharedPtr<Ambient> ambient)
	: RenderComponent(ambient)
	, _cullEnable(true)
{
	auto shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_point_shadows.glsl");
	_shader = MakeShared<ShaderVariation>(shader);
	_shader->Create();

}
MeshShadowPoint::MeshShadowPoint(SharedPtr<Ambient> ambient,bool enable)
	: RenderComponent(ambient)
	, _cullEnable(enable)
{
	auto shader = GetSubSystem<ResourceSystem>()->GetResource<Shader>("shader/au_point_shadows.glsl");
	_shader = MakeShared<ShaderVariation>(shader);
	_shader->Create();

}

MeshShadowPoint::~MeshShadowPoint()
{
}

void MeshShadowPoint::Destory()
{
	UnloadShadow(SharedFromThis(MeshShadowPoint));
	UnloadOpaque(SharedFromThis(MeshShadowPoint));
}

void MeshShadowPoint::DrawReady()
{
	STRING dir = system_content_dictionary;
	_mesh = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("object/base/Cube.3DS");
	_woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad((dir + "texture/wood.jpg").CStr());

	_shader->Use();
	_shader->SetInt("diffuseTexture", 0);
	_shader->SetInt("shadowMap", 1);
}

void MeshShadowPoint::Start()
{
	RegisterShadow(SharedFromThis(MeshShadowPoint));
	RegisterOpaque(SharedFromThis(MeshShadowPoint));
}

void MeshShadowPoint::DrawShadow()
{
	auto shadowShader = GetSubSystem<Renderer>()->GetShadowRenderer()->GetPointDepthMapShader();
	
	glm::mat4 modelMat;
	if (GetNode())		//if gameObject not empty
		modelMat = GetNode()->GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	shadowShader->SetMat4("model", modelMat);

	if (!_cullEnable)
	{
		glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
		_shader->SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	}

	_mesh->DrawMesh(_shader);

	if (!_cullEnable)
	{
		_shader->SetInt("reverse_normals", 0); // and of course disable it
		glEnable(GL_CULL_FACE);
	}
}
void MeshShadowPoint::Draw()
{

	SharedPtr<tCamera> camera = GetSubSystem<Renderer>()->GetCurrentCameraPtr();
	glm::vec3 lightPos;
	//!!! Temp use one light,and must need light
#pragma warning
	VECTOR<SharedPtr<tLight> >& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	//!!!Temp
	Assert(lights.size() != 0);
	for (VECTOR<SharedPtr<tLight> >::iterator it = lights.begin(); it != lights.end(); it++)
	{
		lightPos = (*it)->GetLightPosition();
		unsigned depthMap = (*it)->GetShadowAssist()->GetDepthMap();

		_shader->Use();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		_shader->SetMat4("projection", projection);
		_shader->SetMat4("view", view);
		// set lighting uniforms
		_shader->SetVec3("viewPos", camera->GetPosition());
		_shader->SetVec3("lightPos", lightPos);
		_shader->SetInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
		_shader->SetFloat("far_plane", (*it)->GetFarPlane());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

		glm::mat4 modelMat;
		if (GetNode())		//if gameObject not empty
			modelMat = GetNode()->GetComponent<Transform>()->GetTransformMat();
		else
			modelMat = Matrix4x4::identity;
		_shader->SetMat4("model", modelMat);

		if (!_cullEnable)
		{
			glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
			_shader->SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
		}

		_mesh->DrawMesh(_shader);

		if (!_cullEnable)
		{
			_shader->SetInt("reverse_normals", 0); // and of course disable it
			glEnable(GL_CULL_FACE);
		}
	}
}

}
