#include "MeshShadowPoint.h"
#include "Renderer.h"
#include "Light.h"
#include "VertexData.h"
#include "Camera.h"
#include "Configs.h"
#include "ResourceSystem.h"
#include "Model.h"
namespace Auto3D {

MeshShadowPoint::MeshShadowPoint(Ambient* ambient)
	: RenderComponent(ambient)
	, _shader(shader_path + "au_point_shadows.auvs"
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
	, _shader(shader_path + "au_point_shadows.auvs"
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
	_model = GetSubSystem<ResourceSystem>()->ModelLoad("../Resource/object/base/Cube.3DS");
	_woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/wood.jpg");

	_shader.Use();
	_shader.SetInt("diffuseTexture", 0);
	_shader.SetInt("shadowMap", 1);
}

void MeshShadowPoint::DrawShadow()
{
	Shader& shadowShader = GetSubSystem<Renderer>()->GetShadowRenderer()->GetPointDepthMapShader();
	
	glm::mat4 modelMat;
	if (GetNodePtr())		//if gameObject not empty
		modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	shadowShader.SetMat4("model", modelMat);

	if (!_cullEnable)
	{
		glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
		_shader.SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
	}

	_model->Draw(_shader);

	if (!_cullEnable)
	{
		_shader.SetInt("reverse_normals", 0); // and of course disable it
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

		_shader.Use();
		glm::mat4 projection = camera->GetProjectionMatrix();
		glm::mat4 view = camera->GetViewMatrix();
		_shader.SetMat4("projection", projection);
		_shader.SetMat4("view", view);
		// set lighting uniforms
		_shader.SetVec3("viewPos", camera->GetPosition());
		_shader.SetVec3("lightPos", lightPos);
		_shader.SetInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
		_shader.SetFloat("far_plane", (*it)->GetFarPlane());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap);

		glm::mat4 modelMat;
		if (GetNodePtr())		//if gameObject not empty
			modelMat = GetNode().GetComponent<Transform>()->GetTransformMat();
		else
			modelMat = Matrix4x4::identity;
		_shader.SetMat4("model", modelMat);

		if (!_cullEnable)
		{
			glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
			_shader.SetInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
		}

		_model->Draw(_shader);

		if (!_cullEnable)
		{
			_shader.SetInt("reverse_normals", 0); // and of course disable it
			glEnable(GL_CULL_FACE);
		}
	}
}

}
