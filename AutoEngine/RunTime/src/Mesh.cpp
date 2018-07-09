#include "Mesh.h"
#include "RenderManager.h"
#include "LightManager.h"
AUTO_BEGIN

#if MESH_LIGHT
LightManager& lights = INSTANCE(LightManager);

Mesh::Mesh()
	: m_shader(Shader(AtConfig::shader_path + "au_light_map_model_loading.auvs"
		, AtConfig::shader_path + "au_light_map_model_loading.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = "Resource/object/base/Cube.FBX";

}
Mesh::Mesh(char* meshPath)
	: useStencil(false)
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
	if (m_Material.isTexture)
	{
		m_shader = Shader(AtConfig::shader_path + "au_light_map_model_loading.auvs"
			, AtConfig::shader_path + "au_light_map_model_loading.aufs");
	}
	else
	{
		m_shader = Shader(AtConfig::shader_path + "au_light_model_loading.auvs"
			, AtConfig::shader_path + "au_light_model_loading.aufs");
	}
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
	if (useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		void StencilFunc(GLenum func, GLint ref, GLuint mask);
		void StencilMask(GLuint mask);
		if (m_sfail)
			glStencilOp(m_sfail, m_dpfail, m_dppass);
		else
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if (m_func)
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
	if (cam->GetSortMode() == kSortPerspective)
	{
		projectionMat = glm::perspective(cam->Zoom,
			((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height),
			cam->Near, cam->Far);
	}
	else if (cam->GetSortMode() == kSortOrthographic)
	{
		float t = ((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height);
		projectionMat = glm::ortho(-t, t, -1.0f, 1.0f, cam->Near, cam->Far);
	}
	else
	{
		ErrorString("Fail load projection mat");
	}

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

	// shader configuration
	// --------------------
	if (m_Material.isTexture)
	{
		m_shader.SetInt("material.color", 0);
	}
	else
	{
		m_shader.SetVec3("material.color", m_Material.color.r, m_Material.color.g, m_Material.color.b);
	}
	m_shader.SetVec3("material.diffuse", m_Material.diffuse);
	m_shader.SetVec3("material.specular", m_Material.specular);
	m_shader.SetFloat("material.shininess", m_Material.shininess);
}

void Mesh::drawLight()
{
	auto& t_lights = lights.GetAllLights();
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = t_lights.begin(); it != t_lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition = t->GetGameObject().GetComponent(Transform).GetPosition();
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
			m_shader.SetVec3("pointLight[" + to_string(point) + "].color", t->color);
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
#else
#if MESH_DEBUG
LightManager& lights = INSTANCE(LightManager);

Mesh::Mesh()
	: m_shader(Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs", AtConfig::shader_path + "au_test_light_model_loading.aufs"))
	, m_shadowShader(AtConfig::shader_path + "au_shadow_mapping_depth.auvs", AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = "Resource/object/base/Cube.FBX";
}
Mesh::Mesh(char* meshPath)
	: m_shader(Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs", AtConfig::shader_path + "au_test_light_model_loading.aufs"))
	, m_shadowShader(AtConfig::shader_path + "au_shadow_mapping_depth.auvs", AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = meshPath;
}
Mesh::Mesh(char* meshPath, const Shader& shader)
	: m_shader(shader)
	, m_shadowShader(AtConfig::shader_path + "au_shadow_mapping_depth.auvs", AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
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

	glGenFramebuffers(1, &m_depthMapFBO);
	// create depth texture

	glGenTextures(1, &m_depthMap);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_shader.Use();
	//m_shader.SetInt("diffuseTexture", 0);
	m_shader.SetInt("shadowMap", 0);
}
void Mesh::DrawShadow(const Shader& shader)
{
	glm::mat4 modelMat;
	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;
	shader.SetMat4("model", modelMat);
	m_Model.Draw(shader);
}
void Mesh::Draw(Camera * cam)
{
	if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
	

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
	if (useStencil)
	{
		glEnable(GL_STENCIL_TEST);
		void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		void StencilFunc(GLenum func, GLint ref, GLuint mask);
		void StencilMask(GLuint mask);
		if (m_sfail)
			glStencilOp(m_sfail, m_dpfail, m_dppass);
		else
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		if (m_func)
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
	if (cam->GetSortMode() == kSortPerspective)
	{
		projectionMat = glm::perspective(cam->Zoom,
			((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height),
			cam->Near, cam->Far);
	}
	else if (cam->GetSortMode() == kSortOrthographic)
	{
		float t = ((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height);
		projectionMat = glm::ortho(-t, t, -1.0f, 1.0f, cam->Near, cam->Far);
	}
	else
	{
		ErrorString("Fail load projection mat");
	}
	//Light
	float nearPlane = 1.0f;
	float farPlane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f)
		, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	m_shadowShader.Use();
	m_shadowShader.SetMat4("lightSpaceMatrix", lightProjection * lightView);
	
	


	// reset view
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	m_shadowShader.SetMat4("model", modelMat);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, woodTexture);
	m_Model.Draw(m_shadowShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 


	//RectInt t = INSTANCE(GLWindow).GetWindowRectInt();
	//glViewport(0, 0, t.width, t.height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// 2. render scene as normal using the generated depth/shadow map  
	//// --------------------------------------------------------------
	//glViewport(0, 0, t.width, t.height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader.Use();
	m_shader.SetMat4("model", modelMat);
	m_shader.SetMat4("view", viewMat);
	m_shader.SetMat4("projection", projectionMat);
	m_shader.SetVec3("viewPos", cam->Position);
	m_shader.SetMat4("lightSpaceMatrix", lightProjection * lightView);
	/**/
	//m_shader.SetFloat("time", glfwGetTime());
	/**/
	if (!m_userShader)
	{
		drawMaterial();
		drawLight();
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_depthMap);
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
	auto& t_lights = lights.GetAllLights();
	int dir = 0;
	int point = 0;
	int spot = 0;
	for (auto it = t_lights.begin(); it != t_lights.end(); it++)
	{
		Light * t = *it;
		Vector3 ligthtPosition = t->GetGameObject().GetComponent(Transform).GetPosition();
		//m_shader.SetVec3("lightPos", ligthtPosition);

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
			m_shader.SetVec3("pointLight[" + to_string(point) + "].color", t->color);
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
#else
LightManager& lights = INSTANCE(LightManager);

Mesh::Mesh()
	: m_shader(Shader(AtConfig::shader_path + "au_light_model_loading.auvs", AtConfig::shader_path + "au_light_model_loading.aufs"))
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = "Resource/object/base/Cube.FBX";
}
Mesh::Mesh(char* meshPath)
	: m_shader(Shader(AtConfig::shader_path + "au_light_model_loading.auvs", AtConfig::shader_path + "au_light_model_loading.aufs"))
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
	if (cam->GetSortMode() == kSortPerspective)
	{
		projectionMat = glm::perspective(cam->Zoom,
			((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height),
			cam->Near, cam->Far);
	}
	else if (cam->GetSortMode() == kSortOrthographic)
	{
		float t = ((float)rect.width * (float)cam->ViewRect.width) /
			((float)rect.height * (float)cam->ViewRect.height);
		projectionMat = glm::ortho(-t, t, -1.0f, 1.0f, cam->Near, cam->Far);
	}
	else
	{
		ErrorString("Fail load projection mat");
	}

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
#endif
#endif
AUTO_END