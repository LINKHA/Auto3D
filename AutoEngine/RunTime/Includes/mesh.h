#ifndef MESH_H_
#define MESH_H_
#include "Shader.h"
#include "ModelCommand.h"
#include "BaseMesh.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "LoadResource.h"
AUTO_BEGIN
class Mesh : public BaseMesh
{
	REGISTER_DERIVED_CLASS(Mesh, BaseMesh);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	Mesh();
	Mesh(_String m_meshPath, const Transform& transform = Transform());
	Mesh(_String m_meshPath, const Shader& shader, const Transform& transform = Transform());

	void draw();

	void pushToRunloop();

private:

	Shader m_shader;
	Transform m_transform;
	ModelCommand model;
	_String m_meshPath;
};
AUTO_END
#endif // !MESH_H_
