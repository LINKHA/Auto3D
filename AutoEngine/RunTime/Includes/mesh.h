#ifndef MESH_H_
#define MESH_H_
#include "Shader.h"
#include "ModelCommand.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Math/Color.h"
#include "GameObject.h"
#include "AuMaterial.h"

#define MESH_DEBUG 0
#define MESH_LIGHT 1

AUTO_BEGIN
#if MESH_LIGHT
class Mesh : public Component
{
	REGISTER_DERIVED_CLASS(Mesh, Component);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(char* meshPath);
	Mesh(char* meshPath, const Shader& shader);

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
	Material m_Material;
	ModelCommand m_Model;
	Ptr(char, m_meshPath);
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
private:
	bool m_userShader;
};
#else
#if MESH_DEBUG

class Mesh : public Component
{
	REGISTER_DERIVED_CLASS(Mesh, Component);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(char* meshPath);
	Mesh(char* meshPath, const Shader& shader);

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
	void DrawShadow(const Shader& shader);
private:
	void drawMaterial();
	void drawLight();
private:
	Shader m_shader;
	Shader m_shadowShader;

	Material m_Material;
	ModelCommand m_Model;
	Ptr(char, m_meshPath);
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	


	GLuint m_depthMapFBO;
	GLuint m_depthMap;
private:
	bool m_userShader;
};
#else
class Mesh : public Component
{
	REGISTER_DERIVED_CLASS(Mesh, Component);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(char* meshPath);
	Mesh(char* meshPath, const Shader& shader);

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
	Material m_Material;
	ModelCommand m_Model;
	Ptr(char,m_meshPath);
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
private:
	bool m_userShader;
};
#endif
#endif
AUTO_END
#endif // !MESH_H_
