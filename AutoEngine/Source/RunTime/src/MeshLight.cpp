#include "MeshLight.h"
#include "Configs.h"
#include "Graphics.h"
#include "Renderer.h"
#include "BaseMesh.h"
#include "Light.h"
#include "ResourceSystem.h"
#include "NewDef.h"
namespace Auto3D {

MeshLight::MeshLight(Ambient* ambient)
	: RenderComponent(ambient)
	, _tshader(shader_path + "au_lighting.auvs"
		, shader_path + "au_lighting.aufs")
{}

MeshLight::~MeshLight()
{
	UnloadOpaque(this);
}
#define HDR_DEBUG 0
void MeshLight::Start()
{
	//woodTexture = LocalTextureLoad("../Resource/texture/bricks.jpg");
	woodTexture = GetSubSystem<ResourceSystem>()->TextureLoad("../Resource/texture/bricks.jpg");

	_tshader.Use();
	_tshader.SetInt("diffuseTexture", 0);
	RegisterOpaque(this);
}

void MeshLight::Draw()
{

	glm::mat4 projection = GetSubSystem<Renderer>()->GetCurrentCamera().GetProjectionMatrix();
	glm::mat4 view = GetSubSystem<Renderer>()->GetCurrentCamera().GetViewMatrix();
	_tshader.Use();
	_tshader.SetMat4("projection", projection);
	_tshader.SetMat4("view", view);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, woodTexture);
	// set lighting uniforms

	VECTOR<Light*> lights = GetSubSystem<Renderer>()->GetLightContainer()->GetAllLights();
	int lightNum = 0;
	for (VECTOR<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
	{
		_tshader.SetVec3("lights[" + KhSTL::ToString(lightNum) + "].Position", (*it)->GetNode().GetPosition());
		_tshader.SetVec3("lights[" + KhSTL::ToString(lightNum) + "].Color", (*it)->GetColorToVec());
		lightNum++;
	}

	_tshader.SetVec3("viewPos", GetSubSystem<Renderer>()->GetCurrentCamera().GetPosition());
	// render tunnel
	glm::mat4 model = glm::mat4();

	if (GetNodePtr())		//if gameObject not empty
		model = GetNode().GetComponent<Transform>()->GetTransformMat();
	else
		model = Matrix4x4::identity;

	_tshader.SetMat4("model", model);
	_tshader.SetInt("inverse_normals", true);
	renderCube(&_vao, &_vbo);
}

}
