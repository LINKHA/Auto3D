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
#define MESH_DEBUG 1

AUTO_BEGIN
#if  MESH_DEBUG

class Mesh : public Component
{
	REGISTER_DERIVED_CLASS(Mesh, Component);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(_String m_meshPath);
	Mesh(_String m_meshPath, const Shader& shader);

	void Start();
	void Update();

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
private:

	Shader m_shader;
	ModelCommand m_Model;
	_String m_meshPath;
	Color m_Color;
};
#else
class Mesh : public Motion
{
	REGISTER_DERIVED_CLASS(Mesh, Motion);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(_String m_meshPath);
	Mesh(_String m_meshPath, const Shader& shader);

	void Start();
	void Update();

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
private:

	Shader m_shader;
	ModelCommand m_Model;
	_String m_meshPath;
	Color m_Color;
};
#endif //MESH_DEBUG
AUTO_END
#endif // !MESH_H_
