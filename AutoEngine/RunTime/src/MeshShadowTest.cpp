#include "MeshShadowTest.h"
#include "RenderManager.h"
#include "LightManager.h"
#include "ShadowTest.h"
AUTO_BEGIN
#define DebugShoadow 1
LightManager& m_lights = INSTANCE(LightManager);

unsigned int t_woodTexture;

float s_vertices[] = {
	// back face
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
	1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
	// front face
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
	1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	// left face
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	// right face
	1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
	1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
	// bottom face
	-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
	1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	// top face
	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
	1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
};
MeshShadowTest::MeshShadowTest()
	: m_ShadowMapDepth(AtConfig::shader_path + "au_shadow_mapping_depth.auvs"
		, AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = "Resource/object/base/Cube.FBX";

}
MeshShadowTest::MeshShadowTest(char* meshPath)
	: m_ShadowMapDepth(AtConfig::shader_path + "au_shadow_mapping_depth.auvs"
		, AtConfig::shader_path + "au_shadow_mapping_depth.aufs")
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(false)
{
	m_meshPath.ptr = meshPath;

}
MeshShadowTest::MeshShadowTest(char* meshPath, const Shader& shader)
	: m_shader(shader)
	, useStencil(false)
	, useDepth(true)
	, useBlend(false)
	, m_userShader(true)
{
	m_meshPath.ptr = meshPath;
}
MeshShadowTest::~MeshShadowTest()
{
}
void MeshShadowTest::Start()
{
#if DebugShoadow
	m_shader = Shader(AtConfig::shader_path + "au_shadow_mapping.auvs"
		, AtConfig::shader_path + "au_shadow_mapping.aufs");
#else
	m_shader = Shader(AtConfig::shader_path + "au_test_light_model_loading.auvs"
		, AtConfig::shader_path + "au_test_light_model_loading.aufs");
#endif
	//////////////////////////////////////////////////////////////////////////
#if DebugShoadow
		
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
#else
	m_Model = LocalModelLoad(m_meshPath.ptr);
#endif
	t_woodTexture = LocalTextureLoad("Resource/texture/wood.jpg");
	m_shader.Use();
	m_shader.SetInt("diffuseTexture", 0);
	m_shader.SetInt("shadowMap", 1);
}
void MeshShadowTest::Draw(Camera * cam)
{
	glm::mat4 modelMat;
	glm::mat4 viewMat;
	glm::mat4 projectionMat;
	/*if (cam == nullptr)
	{
		WarningString("Fail to find camera");
		return;
	}
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
	glCullFace(GL_BACK);*/
	/////////////////////////////////////////////////////////////////////////////////////////////
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	
	glViewport(0, 0, INSTANCE(ShadowTest).shadowWidth, INSTANCE(ShadowTest).shadowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, INSTANCE(ShadowTest).depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_woodTexture);
	//glBindTexture(GL_TEXTURE_2D, m_Material.diffuseMap);

	m_ShadowMapDepth.Use();
	m_ShadowMapDepth.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	if (GetGameObjectPtr())		//if gameObject not empty
		modelMat = GetGameObject().GetComponent(Transform).GetTransformMat();
	else
		modelMat = Matrix4x4::identity;

	m_ShadowMapDepth.SetMat4("model", modelMat);
#if DebugShoadow
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
#else
	m_Model.Draw(m_ShadowMapDepth);
#endif
	/////////////////////////////////////////////////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// reset viewport
	RectInt t = INSTANCE(GLWindow).GetWindowRectInt();
	//glViewport(0, 0, t.width, t.height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// 2. render scene as normal using the generated depth/shadow map  
	//// --------------------------------------------------------------
	//glViewport(0, 0, t.width, t.height);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	m_shader.Use();
	
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
	m_shader.SetVec3("lightPos", glm::vec3(-2.0f, 4.0f, -1.0f));
	m_shader.SetMat4("lightSpaceMatrix", lightSpaceMatrix);

	/*if (!m_userShader)
	{
		drawMaterial();
		drawLight();
	}*/
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t_woodTexture);
	//glBindTexture(GL_TEXTURE_2D, m_Material.diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, INSTANCE(ShadowTest).depthMap);
#if DebugShoadow
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
#else
	m_Model.Draw(m_shader);
#endif
/*	if (useStencil)
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
	glDisable(GL_CULL_FACE);*/
}

void MeshShadowTest::drawMaterial()
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

void MeshShadowTest::drawLight()
{
	auto& t_lights = m_lights.GetAllLights();
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
void MeshShadowTest::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	m_sfail = sfail;
	m_dpfail = dpfail;
	m_dppass = dppass;
}
void MeshShadowTest::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	m_func = func;
	m_ref = ref;
	m_mask = mask;
}
void MeshShadowTest::StencilMask(GLuint mask)
{
	m_mas = mask;
}
void MeshShadowTest::DepthFunc(GLenum func)
{
	m_depthfunc = func;
}
AUTO_END