#include "Transform.h"

AUTO_BEGIN
void Transform::translate(const Vector3& position)
{
	m_transform = glm::translate(m_transform, glm::vec3(position.x, position.y, position.z));
}
void Transform::rotation(const Vector3& Euler)
{
	m_rotation.setValueWithEuler(Euler);
}
void Transform::rotation(float Angle, const Vector3& axis)
{
	m_rotation.setValueWithAngleAxis(Angle, axis);
}
void Transform::scale(const Vector3& scale)
{
	m_transform = glm::scale(m_transform, glm::vec3(scale.x, scale.y, scale.z));
}
//public
void Transform::setPosition(const Vector3& position)
{
	m_position = position;
}
void Transform::setRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
}
void Transform::setRotation(const Vector3& euler)
{
	m_rotation.setValueWithEuler(euler);
}
void Transform::setRotation(float Angle, const Vector3& axis)
{
	m_rotation.setValueWithAngleAxis(Angle, axis);
}
void Transform::setScale(const Vector3& scale)
{
	m_scale = scale;
}
void Transform::setScale(float scale)
{
	m_scale = Vector3(scale);
}

glm::mat4 Transform::getTransformMat()
{
	return m_transform;
}
void Transform::updateTransform()
{
	translate(m_position);
	m_transform *= m_rotation.toMatrix4();
	scale(m_scale);
}

void Transform::identity()
{
	m_transform = Matrix4x4::identity;
}



AUTO_END


