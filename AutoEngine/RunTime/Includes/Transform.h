#pragma once
#include "Math/Math.h"
#include "GLGather.h"
#include "GameObject.h"

namespace Auto3D {
class Transform : public Component
{
	REGISTER_DERIVED_CLASS(Transform, Component);
	DECLARE_OBJECT_SERIALIZE(Transform);
private:
	Vector3 _position;
	Quaternion _rotation ;
	Vector3 _scale;
	glm::mat4 _transform;
	
protected:
	void Translate(const Vector3& position);
	void Rotation(const Vector3& Euler);
	void Rotation(float Angle, const Vector3& axis);
	void Scale(const Vector3& scale);
public:
	
	Transform();
	void SetPosition(const Vector3& position);
	void SetPosition(float x, float y, float z);
	void SetRotation(const Quaternion& rotation);
	void SetRotation(const Vector3& euler);
	void SetRotation(float Angle, const Vector3& axis);
	void SetScale(const Vector3& scale);
	void SetScale(float scale);
	void SetScale(float scaleX, float scaleY, float scaleZ);
	Vector3& GetPosition();
	Quaternion& GetRotation();
	Vector3& GetScale();

	glm::mat4& GetTransformMat();
	void UpdateTransform();
	void Identity();

};
}




