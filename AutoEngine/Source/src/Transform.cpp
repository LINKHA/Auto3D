#include "Transform.h"
#include "Ambient.h"
#include "NewDef.h"
namespace Auto3D {

Transform::Transform(SharedPtr<Ambient> ambient)
	:Super(ambient)
	, _position(Vector3F())
	, _rotation(QuaternionF())
	, _scale(Vector3F(1.0f,1.0f,1.0f))
{}

Transform::~Transform()
{}

void Transform::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Transform>(SCENE_ATTACH);
}
void Transform::Rotation(const Vector3F& Euler)
{
	//_rotation.SetValueWithEuler(Euler);
}

void Transform::Rotation(float Angle, const Vector3F& axis)
{
	//_rotation.SetValueWithAngleAxis(Angle, axis);
}

void Transform::SetPosition(const Vector3F& position)
{
	_position = position;
}

void Transform::SetPosition(float x,float y,float z)
{
	_position = Vector3F(x, y, z);
}

void Transform::SetRotation(const QuaternionF& rotation)
{
	_rotation = rotation;
}

void Transform::SetRotation(const Vector3F& euler)
{
	//_rotation.SetValueWithEuler(euler);
}

void Transform::SetRotation(float Angle, const Vector3F& axis)
{
	//_rotation.SetValueWithAngleAxis(Angle, axis);
}

void Transform::SetScale(const Vector3F& scale)
{
	setScaleAbs(scale._x, scale._y, scale._z);
}

void Transform::SetScale(float scale)
{
	setScaleAbs(scale, scale, scale);
}

void Transform::SetScale(float scaleX, float scaleY, float scaleZ)
{
	setScaleAbs(scaleX, scaleY, scaleZ);
}

Vector3F Transform::GetPosition()
{
	return _position;
}

QuaternionF Transform::GetRotation()
{
	return _rotation;
}

Vector3F Transform::GetScale()
{
	return _scale;
}

glm::mat4 Transform::GetTransformMat()
{
	glm::mat4 modelMat = glm::mat4();
	//Convert to left-handed coordinate system
	modelMat = glm::translate(modelMat, glm::vec3(-_position._x, _position._y, _position._z));
	//modelMat *= _rotation.toMatrix4();
	modelMat = glm::scale(modelMat, glm::vec3(_scale._x,_scale._y, _scale._z));

	return modelMat;
}

void Transform::setScaleAbs(float x, float y, float z)
{
	_scale = Vector3F(Auto3D::Abs(x), Auto3D::Abs(y), Auto3D::Abs(z));
}


}
