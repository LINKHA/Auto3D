#include "MeshShadowPoint.h"
#include "Renderer.h"
#include "Light.h"
#include "VertexData.h"
#include "Camera.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "Mesh.h"
#include "NewDef.h"

namespace Auto3D {

MeshShadowPoint::MeshShadowPoint(Ambient* ambient)
	: RenderComponent(ambient)
	, _tshader(shader_path + "au_point_shadows.auvs"
		, shader_path + "au_point_shadows.aufs")
	//, _hardShader(Shader(shader_path + "au_hard_point_shadows.auvs"	//!!! Temp not hard shader
	//	, shader_path + "au_point_shadows.aufs"))
	, _cullEnable(true)
{
	RegisterShadow(this);
	RegisterOpaque(this);
}
MeshShadowPoint::MeshShadowPoint(Ambient* ambient,bool enable)
	: RenderComponent(ambient)
	, _tshader(shader_path + "au_point_shadows.auvs"
		, shader_path + "au_point_shadows.aufs")
	//, _hardShader(Shader(shader_path + "au_hard_point_shadows.auvs"	//!!! Temp not hard shader
	//	, shader_path + "au_point_shadows.aufs"))
	, _cullEnable(enable)
{
	RegisterShadow(this);
	RegisterOpaque(this);
}

MeshShadowPoint::~MeshShadowPoint()
{
	UnloadShadow(this);
	UnloadOpaque(this);
}
void MeshShadowPoint::DrawReady()
{
	Mesh* tmp = GetSubSystem<ResourceSystem>()->GetResource<Mesh>("../Resource/object/base/Cube.3DS");
	_mesh.reset(tmp);
	_woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/wood.jpg");

	_tshader.Use();
	_tshader.SetInt("diffuseTexture", 0);
	_tshader.SetInt("shadowMap", 1);
}

void MeshShadowPoint::DrawShadow()
{
	_Shader& shadowShader = GetSubSystem<Renderer>()->GetShadowRenderer()->GetPointDepthMapShader();
	
	glm::mat4 modelMat;
	if (GetNodePtr())		//if gameObject not empty
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	shadowShader.SetMat4("model", modelMat);

	if (!_cullEnable)
	{
		glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
		_tshader.SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	}

	_mesh->DrawMesh(_tshader);

	if (!_cullEnable)
	{
		_tshader.SetInt("reverse_normals", 0); // and of course disable it
		glEnable(GL_CULL_FACE);
	}
}
void MeshShadowPoint::Draw()
{

	Camera* camera = GetSubSystem<Renderer>()->GetCurrentCameraPtr();
	glm::vec3 lightPos;
	//!!! Temp use one light,and must need light
#pragma warning
	VECTOR<Light*>& lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	//!!!Temp
	Assert(lights.size() != 0);
	for (VECTOR<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		lightPos = (*it)->GetLightPosition();
		unsigned depthMap = (*it)->GetShadowAssist()->GetDepthMap();

		_tshader.Use();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		_tshader.SetMat4("projection", projection);
		_tshader.SetMat4("view", view);
		// set lighting uniforms
		_tshader.SetVec3("viewPos", camera->GetPosition());
		_tshader.SetVec3("lightPos", lightPos);
		_tshader.SetInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
		_tshader.SetFloat("far_plane", (*it)->GetFarPlane());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

		glm::mat4 modelMat;
		if (GetNodePtr())		//if gameObject not empty
			modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
		else
			modelMat = Matrix4x4::identity;
		_tshader.SetMat4("model", modelMat);

		if (!_cullEnable)
		{
			glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
			_tshader.SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
		}

		_mesh->DrawMesh(_tshader);

		if (!_cullEnable)
		{
			_tshader.SetInt("reverse_normals", 0); // and of course disable it
			glEnable(GL_CULL_FACE);
		}
	}
}

}
