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
	glm::mat4 m_transform;
	Quaternion m_rotation = Quaternion();
protected:
	void translate(const Vector3& position);
	void rotation(const Vector3& Euler);
	void rotation(float Angle, const Vector3& axis);
	void scale(const Vector3& scale);
public:
	
	Transform(){}
	void setPosition(const Vector3& position);
	void setRotation(const Quaternion& rotation);
	void setRotation(const Vector3& euler);
	void setRotation(float Angle, const Vector3& axis);
	void setScale(const Vector3& scale);
	void setScale(float scale);
	
	glm::mat4 getTransformMat();
	void updateTransform();

	void identity();

};
AUTO_END
#endif // TRANSFORM_H_