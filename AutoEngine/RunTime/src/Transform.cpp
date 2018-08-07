#include "Transform.h"

namespace Auto3D {

Transform::Transform()
	: _position(Vector3())
	, _rotation(Quaternion())
	, _scale(Vector3(1.0f))
{}
Transform::~Transform()
{}
void Transform::Rotation(const Vector3& Euler)
{
	_rotation.SetValueWithEuler(Euler);
}
void Transform::Rotation(float Angle, const Vector3& axis)
{
	_rotation.SetValueWithAngleAxis(Angle, axis);
}
void Transform::SetPosition(const Vector3& position)
{
	_position = position;
}
void Transform::SetPosition(float x,float y,float z)
{
	_position.Set(x, y, z);
}

void Transform::SetRotation(const Quaternion& rotation)
{
	_rotation = rotation;
}
void Transform::SetRotation(const Vector3& euler)
{
	_rotation.SetValueWithEuler(euler);
}
void Transform::SetRotation(float Angle, const Vector3& axis)
{
	_rotation.SetValueWithAngleAxis(Angle, axis);
}
void Transform::SetScale(const Vector3& scale)
{
	_scale = scale;
}
void Transform::SetScale(float scale)
{
	_scale.Set(scale, scale, scale);
}
void Transform::SetScale(float scaleX, float scaleY, float scaleZ)
{
	_scale.Set(scaleX, scaleY, scaleZ);
}
Vector3& Transform::GetPosition()
{
	return _position;
}
Quaternion& Transform::GetRotation()
{
	return _rotation;
}
Vector3& Transform::GetScale()
{
	return _scale;
}

glm::mat4 Transform::GetTransformMat()
{
	glm::mat4 modelMat = glm::mat4();
	modelMat = glm::translate(modelMat, glm::vec3(_position.x, _position.y, _position.z));
	modelMat *= _rotation.toMatrix4();
	modelMat = glm::scale(modelMat, glm::vec3(_scale.x, _scale.y, _scale.z));
	return modelMat;
	//return _transform;
}

}
