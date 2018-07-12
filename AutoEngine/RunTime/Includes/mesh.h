#ifndef MESH_H_
#define MESH_H_
#include "Shader.h"
#include "ModelCommand.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Math/Color.h"
#include "AuMaterial.h"
#include "RanderComponent.h"
#include "GLMeshEnable.h"

AUTO_BEGIN

class Mesh : public RanderComponent ,public GLMeshEnable
{
	REGISTER_DERIVED_CLASS(Mesh, RanderComponent);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(char* meshPath);
	Mesh(char* meshPath, const Shader& shader);

	void Start()override;
	void Draw(Camera * cam = nullptr)override;
	Material& GetMaterial() { return m_Material; }

private:
	void drawMaterial();
	void drawLight();
private:
	Shader m_shader;
	Material m_Material;
	ModelCommand m_Model;
	Ptr(char, m_meshPath);
private:
	bool m_userShader;
};

AUTO_END
#endif // !MESH_H_
