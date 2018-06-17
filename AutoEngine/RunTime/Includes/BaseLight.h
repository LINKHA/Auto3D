#ifndef BASE_LIGHT_H_
#define BASE_LIGHT_H_
#include "Math/Vector3.h"
#include "Math/Color.h"
#include "GameObject.h"
#include "Shader.h"
#include "ModelCommand.h"
#include "LightManager.h"
#include "Transform.h"
USING_MATH
AUTO_BEGIN

enum LightType
{
	Directional,
	Point,
	Spot
};

class Light : public Component
{
	REGISTER_DERIVED_CLASS(Light, Component);
	DECLARE_OBJECT_SERIALIZE(Light);
public:
	Light();
	Light(LightType type);
	LightType GetType() const { return static_cast<LightType>(m_Type); }

	void AddToManager();
	void RemoveFromManager();
	void ComputeLight();

	Color color;

	Vector3 direction;

	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;

	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;

private:
	ModelCommand		model;
	int					m_Type;						//Light Type
};

AUTO_END
#endif //!BASE_LIGHT_H_
