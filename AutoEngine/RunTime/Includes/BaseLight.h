#ifndef BASE_LIGHT_H_
#define BASE_LIGHT_H_
#include "Math/Vector3.h"
#include "Math/Color.h"
#include "GameObject.h"
#include "Shader.h"
#include "ModelCommand.h"
USING_MATH
AUTO_BEGIN

class BaseLight : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(BaseLight, Component);
	DECLARE_OBJECT_SERIALIZE(BaseLight);
public:
	BaseLight();
	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
	Color GetColor();
private:
	Shader m_Shader;
	ModelCommand model;
	Color m_Color;
};

AUTO_END
#endif //!BASE_LIGHT_H_
