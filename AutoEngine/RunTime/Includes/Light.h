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
enum ShadowType
{
	SHADOW_TYPE_NO_SHADOW,
	SHADOW_TYPE_SOFT,
	SHADOW_TYPE_HARD
};
/**
* @brief : Manage this light create shadow
*/
class ShadowRenderAssist : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(ShadowRenderAssist, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(ShadowRenderAssist);
public:
	explicit ShadowRenderAssist(Ambient* ambient);
	/**
	* @brief : Bind directional light create depath map
	*/
	void BindDirDepathMap();
	/**
	* @brief : Bind point light create depath map
	*/
	void BindPointDepathMap();
private:
	int _shadowWidth;
	int _shadowHeight;

	unsigned int _depthDirMapFBO;
	unsigned int _depthDirMap;

	unsigned int _depthPointMapFBO;
	unsigned int _depthPointmap;
};
/**
* @brief : Light component
*/
class Light : public Component
{
	REGISTER_DERIVED_CLASS(Light, Component);
	DECLARE_OBJECT_SERIALIZE(Light);
public:
	Light();
	explicit Light(Ambient* ambi);
	/**
	* @brief : Update light data
	*/
	void Update()override;
	/**
	* @brief : Get light type
	*/
	LightType GetType() const { return static_cast<LightType>(_type); }
	/**
	* @brief : Set shadow type and create shadow assist
	*/
	void SetShadowType(ShadowType type);
	/**
	* @brief : Get light space matrix (projection * view)
	*/
	glm::mat4& GetLightSpaceMatrix();
	/**
	* @brief : Get light projection matrix (ortho)
	*/
	glm::mat4& GetLightProjectionMatrix();
	/**
	* @brief : Get light space matrix (lookAt Vector::ZERO)
	*/
	glm::mat4& GetLightViewMatrix();


	void AddToManager();
	void RemoveFromManager();


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

protected:	
	ShadowRenderAssist* _shadowAssist;
	ShadowType _shadowType;
	glm::mat4 _lightSpaceMatrix;
	glm::mat4 _lightProjectionMatrix;
	glm::mat4 _lightViewMatrix;
	///shadow far plane
	float _farPlane;
	///light type
	int	_type;
};

}

