#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "Math/AUMath.h"
#include "OpenGLGather.h"
#include "Motion.h"
USING_MATH
AUTO_BEGIN
class Transform : public Motion
{
	REGISTER_DERIVED_CLASS(Transform, Motion);
	DECLARE_OBJECT_SERIALIZE(Transform);
private:
	Vector3 m_position;
	Quaternion m_rotation ;
	Vector3 m_scale;
	glm::mat4 m_transform;
	
protected:
	void Translate(const Vector3& position);
	void Rotation(const Vector3& Euler);
	void Rotation(float Angle, const Vector3& axis);
	void Scale(const Vector3& scale);
public:
	
	Transform();
	void SetPosition(const Vector3& position);
	void SetRotation(const Quaternion& rotation);
	void SetRotation(const Vector3& euler);
	void SetRotation(float Angle, const Vector3& axis);
	void SetScale(const Vector3& scale);
	void SetScale(float scale);

	Vector3 GetPosition();
	Quaternion GetRotation();
	Vector3 GetScale();

	glm::mat4 GetTransformMat();
	void UpdateTransform();
	void Identity();

	void Start() {}
	void Update() {}
};
AUTO_END
#endif // TRANSFORM_H_



