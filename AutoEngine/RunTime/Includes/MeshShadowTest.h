#pragma once
#include "Shader.h"
#include "ModelCommand.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Math/Color.h"
#include "GameObject.h"
#include "AuMaterial.h"

AUTO_BEGIN
class MeshShadowTest : public Component
{
public:
	MeshShadowTest();
	MeshShadowTest(char* meshPath);
	MeshShadowTest(char* meshPath, const Shader& shader);
	~MeshShadowTest();


	void Start()override;
	void Draw(Camera * cam = nullptr)override;
	Material& GetMaterial() { return m_Material; }

	bool useStencil;
	bool useDepth;
	bool useBlend;
	void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void StencilFunc(GLenum func, GLint ref, GLuint mask);
	void StencilMask(GLuint mask);
	void DepthFunc(GLenum func);
private:
	void drawMaterial();
	void drawLight();
private:
	Shader m_shader;
	Shader m_ShadowMapDepth;
	Material m_Material;
	ModelCommand m_Model;
	Ptr(char, m_meshPath);
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
private:
	bool m_userShader;
	unsigned int cubeVAO;
	unsigned int cubeVBO;
};

AUTO_END