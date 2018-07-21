#pragma once
#include "Math.h"
#include "Auto.h"
#include "glm/gtx/quaternion.hpp"
MATH_BEGIN


struct Quaternion
{
	glm::quat value;

	Quaternion() { value = glm::quat(); }
	explicit Quaternion(const Vector3& euler);
	Quaternion(float angle, const Vector3& axis);

	void SetValueWithEuler(const Vector3& euler);
	void SetValueWithAngleAxis(float angle, const Vector3& axis);

	glm::mat4 toMatrix4()
	{
		return glm::toMat4(value);
	}
	

};


MATH_END


