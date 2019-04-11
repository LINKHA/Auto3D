#include "PhysicsUtils.h"

namespace Auto3D
{

btVector3 ToBtVector3(const Vector3F& vector)
{
	return btVector3(vector._x, vector._y, vector._z);
}

btQuaternion ToBtQuaternion(const Quaternion& quaternion)
{
	return btQuaternion(quaternion._x, quaternion._y, quaternion._z, quaternion._w);
}

Vector3F ToVector3(const btVector3& vector)
{
	return Vector3F(vector.x(), vector.y(), vector.z());
}

Quaternion ToQuaternion(const btQuaternion& quaternion)
{
	return Quaternion(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}

bool HasWorldScaleChanged(const Vector3F& oldWorldScale, const Vector3F& newWorldScale)
{
	Vector3F delta = newWorldScale - oldWorldScale;
	float dot = Dot(delta, delta);
	return dot > 0.01f;
}

Color ToColor(const b2Color& color)
{
	return Color(color.r, color.g, color.b);
}

b2Vec2 ToB2Vec2(const Vector2F& vector)
{
	return { vector._x, vector._y };
}

Vector2F ToVector2(const b2Vec2& vec2)
{
	return Vector2F(vec2.x, vec2.y);
}

b2Vec2 ToB2Vec2(const Vector3F& vector)
{
	return { vector._x, vector._y };
}

Vector3F ToVector3(const b2Vec2& vec2)
{
	return Vector3F(vec2.x, vec2.y, 0.0f);
}

}