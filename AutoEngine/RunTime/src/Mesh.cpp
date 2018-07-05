#include "Mesh.h"
#include "RenderManager.h"
#include "LightManager.h"
AUTO_BEGIN


LightManager& lights = INSTANCE(LightManager);

Mesh::Mesh()
	: m_shader(Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs", AtConfig::shader_path + "au_test_light_model_loading.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = "Resource/object/nanosuit/nanosuit.obj";
}
Mesh::Mesh(char* meshPath)
	: m_shader(Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs", AtConfig::shader_path + "au_test_light_model_loading.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = meshPath;
}
Mesh::Mesh(char* meshPath, const Shader& shader)
	: m_shader(shader)
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(true)
{
	m_meshPath.ptr = meshPath;
}
Mesh::~Mesh()
{
}
void Mesh::Start()
{
	//////////////////////////////////////////////////////////////////////////
	m_Model = LocalModelLoad(m_meshPath.ptr);
}
void Mesh::Draw(Camera * cam)
{
	if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	m_shader.Use();

	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	
	if (useDepth)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(m_depthfunc);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if(useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		void StencilFunc(GLenum func, GLint ref, GLuint mask);
		void StencilMask(GLuint mask);
		if(m_sfail)
			glStencilOp(m_sfail, m_dpfail, m_dppass);
		else 
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if(m_func)
			glStencilFunc(m_func, m_ref, m_mask);
		else
			glStencilFunc(GL_ALWAYS, 1, 0xFF);

			glStencilMask(m_mas);

	}
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	viewMat = cam->GetViewMatrix();

	RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
	projectionMat = glm::perspective(cam->Zoom,
		((float)rect.width * (float)cam->ViewRect.width) /
		((float)rect.height * (float)cam->ViewRect.height),
		cam->Near, cam->Far);

	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec3("viewPos", cam->Position);
	/**/
	//m_shader.SetFloat("time", glfwGetTime());
	/**/
	if (!m_userShader)
	{
		drawMaterial();
		drawLight();
	}
	m_Model.Draw(m_shader);
	if (useStencil)
	{
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glDisable(GL_STENCIL_TEST);
		glStencilMask(0xFF);
	}
	if (!useDepth)
		glEnable(GL_DEPTH_TEST);
	else 
		glDepthFunc(GL_LESS);

	glCullFace(GL_FRONT);	
	glDisable(GL_CULL_FACE);	
}

void Mesh::drawMaterial()
{
	m_shader.SetVec3("material.color", m_Material.color.r, m_Material.color.g, m_Material.color.b);
	m_shader.SetVec3("material.diffuse", m_Material.diffuse);
	m_shader.SetVec3("material.specular", m_Material.specular);
	m_shader.SetFloat("material.shininess", m_Material.shininess);
}

void Mesh::drawLight()
{
	auto& t_lights= lights.GetAllLights();
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = t_lights.begin(); it != t_lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition= t->GetGameObject().GetComponent(Transform).GetPosition();
		m_shader.SetVec3("lightPos", ligthtPosition);

		switch (t->GetType())
		{
			case Directional:
				if (dir >= 4)break;
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].color", t->color);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].direction", t->direction);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].ambient", t->ambient);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].diffuse", t->diffuse);
				m_shader.SetVec3("dirLight[" + to_string(dir) + "].specular", t->specular);
				dir++;
				break;
			case Point:
				if (point >= 8)break;
				m_shader.SetVec3("pointLight[" + to_string( point) + "].color", t->color);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].position", ligthtPosition);
				m_shader.SetFloat("pointLight[" + to_string(point) + "].constant", t->constant);
				m_shader.SetFloat("pointLight[" + to_string(point) + "].linear", t->linear);
				m_shader.SetFloat("pointLight[" + to_string(point) + "].quadratic", t->quadratic);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].ambient", t->ambient);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].diffuse", t->diffuse);
				m_shader.SetVec3("pointLight[" + to_string(point) + "].specular", t->specular);
				point++;
				break;
			case Spot:
				
				if (spot >= 4)break;
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].color", t->color);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].position", ligthtPosition);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].direction", t->direction);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].cutOff", t->cutOff);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].outerCutOff", t->outerCutOff);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].constant", t->constant);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].linear", t->linear);
				m_shader.SetFloat("spotLight[" + to_string(spot) + "].quadratic", t->quadratic);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].ambient", t->ambient);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].diffuse", t->diffuse);
				m_shader.SetVec3("spotLight[" + to_string(spot) + "].specular", t->specular);
				spot++;
				break;
			default:
				WarningString("Fail to set light!");
		}
	}
	m_shader.SetBool("dirBool", (bool)dir);
	m_shader.SetBool("pointBool", (bool)point);
	m_shader.SetBool("spotBool", (bool)spot);

	m_shader.SetInt("dirNum", dir);
	m_shader.SetInt("pointNum", point);
	m_shader.SetInt("spotNum", spot);
}
void Mesh::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail; 
	m_dpfail = dpfail; 
	m_dppass = dppass;
}
void Mesh::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func; 
	m_ref = ref;
	m_mask = mask;
}
void Mesh::StencilMask(GLuint mask) 
{
	m_mas = mask;
}
void Mesh::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}
AUTO_END