#pragma once
#include "Math/Vector3.h"
#include "Math/Color.h"
#include "GameObject.h"
#include "Shader.h"
#include "ModelCommand.h"
#include "LightManager.h"
#include "Transform.h"
#include "ShadowSetting.h"
namespace Auto3D {

enum LightType
{
	kDirectional,
	kPoint,
	kSpot
};

class Light : public Component
{
	REGISTER_DERIVED_CLASS(Light, Component);
	DECLARE_OBJECT_SERIALIZE(Light);
public:
	Light();
	explicit Light(Ambient* ambi);
	LightType GetType() const { return static_cast<LightType>(_type); }

	void AddToManager();
	void RemoveFromManager();

	ShadowSetting* _shadowSetting;

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

	float nearPlane;
	float farPlane;

protected:
	int					_type;
};

}

