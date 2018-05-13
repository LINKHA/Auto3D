#include "Transform.h"

AUTO_BEGIN

Transform::Transform()
	: m_position(Vector3())
	, m_rotation(Quaternion())
	, m_scale(Vector3(1.0f))
{}
void Transform::Translate(const Vector3& position)
{
	m_transform = glm::translate(m_transform, glm::vec3(position.x, position.y, position.z));
}
void Transform::Rotation(const Vector3& Euler)
{
	m_rotation.SetValueWithEuler(Euler);
}
void Transform::Rotation(float Angle, const Vector3& axis)
{
	m_rotation.SetValueWithAngleAxis(Angle, axis);
}
void Transform::Scale(const Vector3& scale)
{
	m_transform = glm::scale(m_transform, glm::vec3(scale.x, scale.y, scale.z));
}
//public
void Transform::SetPosition(const Vector3& position)
{
	m_position = position;
}
void Transform::SetRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
}
void Transform::SetRotation(const Vector3& euler)
{
	m_rotation.SetValueWithEuler(euler);
}
void Transform::SetRotation(float Angle, const Vector3& axis)
{
	m_rotation.SetValueWithAngleAxis(Angle, axis);
}
void Transform::SetScale(const Vector3& scale)
{
	m_scale = scale;
}
void Transform::SetScale(float scale)
{
	m_scale = Vector3(scale);
}

glm::mat4 Transform::GetTransformMat()
{
	return m_transform;
}
void Transform::UpdateTransform()
{
	Translate(m_position);
	m_transform *= m_rotation.toMatrix4();
	Scale(m_scale);
}

void Transform::Identity()
{
	m_transform = Matrix4x4::identity;
}



AUTO_END


