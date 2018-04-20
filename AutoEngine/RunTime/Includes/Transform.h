#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "BaseComponent.h"
#include "Math/AUMath.h"
#include "OpenGLGather.h"

USING_MATH
AUTO_BEGIN
class Transform
{
private:
	
	Vector3 m_position = Vector3();
	
	Vector3 m_scale = Vector3(1.0f);
public:
	glm::mat4 m_transform; 
	Quaternion m_rotation = Quaternion();
	Transform(){}
	void setPosition(const Vector3& position) 
	{
		m_position = position;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	void SetRotation(const Vector3& euler)
	{
		m_rotation.setValueWithEuler(euler);
	}
	void SetRotation(float Angle, const Vector3& axis)
	{
		m_rotation.setValueWithAngleAxis(Angle, axis);
	}
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	void SetScale(float scale)
	{
		m_scale = Vector3(scale);
	}
	/*
	void updateTransform()
	{
		m_transform =translate(m_position);
		m_transform =scale(m_transform, m_scale); // TODO: order is off here for some reason, figure out why
		m_transform =rotate(m_transform, m_rotation.xyz, m_rotation.w);
	}*/

};
static void translate(Transform& tranOperate, const Vector3& position)
{
	tranOperate.m_transform = glm::translate(tranOperate.m_transform, glm::vec3(position.x, position.y, position.z));
}

static void rotation(Transform& tranOperate, const Vector3& Euler)
{
	tranOperate.m_rotation.setValueWithEuler(Euler);
}
static void rotation(Transform& tranOperate, float Angle, const Vector3& axis)
{
	tranOperate.m_rotation.setValueWithAngleAxis(Angle, axis);
}
static void scale(Transform& tranOperate, const Vector3& scale)
{
	tranOperate.m_transform = glm::scale(tranOperate.m_transform, glm::vec3(scale.x, scale.y, scale.z));
}
/*
static void translate(Transform& tranOperate, const Vector3& position)
{
	tranOperate.m_transform = glm::translate(tranOperate.m_transform, glm::vec3(position.x, position.y, position.z));
}

static void rotation(Transform& tranOperate, const Vector3& Euler)
{
	tranOperate.m_rotation.setValueWithEuler(Euler);
}
static void rotation(Transform& tranOperate, float Angle, const Vector3& axis)
{
	tranOperate.m_rotation.setValueWithAngleAxis(Angle, axis);
}
static void scale(Transform& tranOperate, const Vector3& scale)
{
	tranOperate.m_transform = glm::scale(tranOperate.m_transform, glm::vec3(scale.x, scale.y, scale.z));
}
*/
AUTO_END
#endif // TRANSFORM_H_