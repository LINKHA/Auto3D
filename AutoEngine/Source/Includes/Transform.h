#pragma once
#include "AutoMath.h"
#include "AutoOGL.h"
#include "Component.h"
;
namespace Auto3D {
class Transform : public Component
{
	REGISTER_OBJECT_CLASS(Transform, Component)
public:
	/**
	* @brief : Init position(0) rotation(0) and scale(1)
	*/
	explicit Transform(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Set position from Vector3
	*/
	void SetPosition(const Vector3F& position);
	/**
	* @brief : Set position from x,y,z
	*/
	void SetPosition(float x, float y, float z);
	/**
	* @brief : Set Rotation from rotation
	*/
	void SetRotation(const QuaternionF& rotation);
	/**
	* @brief : Set Rotation from euler(Vector3)
	*/
	void SetRotation(const Vector3F& euler);
	/**
	* @brief : Set Rotation from angle and axis
	*/
	void SetRotation(float Angle, const Vector3F& axis);
	/**
	* @brief : Set Scale from Vector3
	*/
	void SetScale(const Vector3F& scale);
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
	Vector3F GetPosition();
	/**
	* @brief : Get Rotation
	* @return : Quaternion&
	*/
	QuaternionF GetRotation();
	/**
	* @brief : Get Scale
	* @return : Vector3&
	*/
	Vector3F GetScale();
	/**
	* @brief : Get transform matrix and update transform matrix data
	* @return : glm::mat4
	*/
	glm::mat4 GetTransformMat();
protected:
	void setScaleAbs(float x, float y, float z);
	void Rotation(const Vector3F& Euler);
	void Rotation(float Angle, const Vector3F& axis);
private:
	Vector3F _position;
	QuaternionF _rotation;
	Vector3F _scale;
};
}




