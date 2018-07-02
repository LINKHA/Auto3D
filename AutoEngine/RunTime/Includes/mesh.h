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

AUTO_BEGIN
class Mesh : public Component
{
	REGISTER_DERIVED_CLASS(Mesh, Component);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(_String m_meshPath);
	Mesh(_String m_meshPath, const Shader& shader);

	void Start();
	void Draw(Camera * cam);
	Material& GetMaterial() { return m_Material; }
	bool useStencil;
	bool useDepth;
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
	_String m_meshPath;
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
};

AUTO_END
#endif // !MESH_H_
