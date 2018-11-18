#include "Quaternion.h"

namespace Auto3D {
Quaternion::Quaternion(const Vector3& euler)
{
	value = glm::quat(glm::vec3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z))); 
}
Quaternion::Quaternion(float angle, const Vector3& axis)
{
	value = glm::angleAxis(glm::degrees(glm::radians(angle)), glm::vec3(axis.x, axis.y, axis.z));
}
void Quaternion::SetValueWithEuler(const Vector3 & euler)
{
	value = glm::quat(glm::vec3(glm::radians(euler.x), glm::radians(euler.y), glm::radians(euler.z)));
}
void Quaternion::SetValueWithAngleAxis(float angle, const Vector3 & axis)
{
	value = glm::angleAxis(glm::degrees(glm::radians(angle)), glm::vec3(axis.x, axis.y, axis.z));
}
}