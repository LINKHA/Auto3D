#pragma once
#include "Math/Math.h"
#include "AutoOGL.h"
#include "Component.h"
;
namespace Auto3D {
class Transform : public Component
{
	REGISTER_DERIVED_CLASS(Transform, Component);
	DECLARE_OBJECT_SERIALIZE(Transform);

public:
	/**
	* @brief : Init position(0) rotation(0) and scale(1)
	*/
	explicit Transform(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	//static void RegisterObject(Ambient* ambient);
	/**
	* @brief : Set position from Vector3
	*/
	void SetPosition(const Vector3& position);
	/**
	* @brief : Set position from x,y,z
	*/
	void SetPosition(float x, float y, float z);
	/**
	* @brief : Set Rotation from rotation
	*/
	void SetRotation(const Quaternion& rotation);
	/**
	* @brief : Set Rotation from euler(Vector3)
	*/
	void SetRotation(const Vector3& euler);
	/**
	* @brief : Set Rotation from angle and axis
	*/
	void SetRotation(float Angle, const Vector3& axis);
	/**
	* @brief : Set Scale from Vector3
	*/
	void SetScale(const Vector3& scale);
	/**
	* @brief : Set Scale from float
	*/
	void SetScale(float scale);
	/**
	* @brief :Set Scale from x,y,z
	*/
	void SetScale(float scaleX, float scaleY, float scaleZ);
	/**
	* @brief : Get Position
	* @return : Vector3&
	*/
	Vector3& GetPosition();
	/**
	* @brief : Get Rotation
	* @return : Quaternion&
	*/
	Quaternion& GetRotation();
	/**
	* @brief : Get Scale
	* @return : Vector3&
	*/
	Vector3& GetScale();
	/**
	* @brief : Get transform matrix and update transform matrix data
	* @return : glm::mat4
	*/
	glm::mat4 GetTransformMat();
protected:
	void setScaleAbs(float x, float y, float z);
	void Rotation(const Vector3& Euler);
	void Rotation(float Angle, const Vector3& axis);
private:
	Vector3 _position;
	Quaternion _rotation;
	Vector3 _scale;
};
}




