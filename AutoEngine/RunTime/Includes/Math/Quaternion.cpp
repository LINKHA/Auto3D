#include "Quaternion.h"

MATH_BEGIN
Quaternion::Quaternion(const Vector3& euler)
{
	value = glm::quat(glm::vec3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z))); 
}
Quaternion::Quaternion(float angle, const Vector3& axis)
{
	value = glm::angleAxis(glm::degrees(glm::radians(angle)), glm::vec3(axis.x, axis.y, axis.z));
}
void Quaternion::setValueWithEuler(const Vector3 & euler)
{
	value = glm::quat(glm::vec3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z)));
}
void Quaternion::setValueWithAngleAxis(float angle, const Vector3 & axis)
{
	value = glm::angleAxis(glm::degrees(glm::radians(angle)), glm::vec3(axis.x, axis.y, axis.z));
}
MATH_END