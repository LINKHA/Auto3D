#ifndef TEXT_MESH_H_
#define TEXT_MESH_H_
#include "Shader.h"
#include "ModelCommand.h"
#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "Math/Color.h"
AUTO_BEGIN
class TextMesh : public Component
{
	REGISTER_DERIVED_CLASS(TextMesh, Component);
	DECLARE_OBJECT_SERIALIZE(TextMesh);
public:
	TextMesh();

	void Draw();
	void PushToRunloop();

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
private:
	unsigned int t_VAO, t_VBO;
	Shader m_shader;
	Color m_Color;
};
AUTO_END
#endif // !TEXT_MESH_H_
