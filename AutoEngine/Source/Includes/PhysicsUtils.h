#pragma once

#include "Color.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "AutoPhysics2D.h"
#include "Bullet/LinearMath/btVector3.h"
#include "Bullet/LinearMath/btQuaternion.h"

namespace Auto3D {

inline btVector3 ToBtVector3(const Vector3& vector)
{
	return btVector3(vector._x, vector._y, vector._z);
}

//inline btQuaternion ToBtQuaternion(const Quaternion& quaternion)
//{
//	return btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
//}

inline Vector3 ToVector3(const btVector3& vector)
{
	return Vector3(vector.x(), vector.y(), vector.z());
}

inline Quaternion ToQuaternion(const btQuaternion& quaternion)
{
	//return Quaternion(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}

inline bool HasWorldScaleChanged(const Vector3& oldWorldScale, const Vector3& newWorldScale)
{
	Vector3 delta = newWorldScale - oldWorldScale;
	float dot = Dot(delta, delta);
	return dot > 0.01f;
}

inline Color ToColor(const b2Color& color)
{
	return Color(color.r, color.g, color.b);
}

inline b2Vec2 ToB2Vec2(const Vector2& vector)
{
	return { vector._x, vector._y };
}

inline Vector2 ToVector2(const b2Vec2& vec2)
{
	return Vector2(vec2.x, vec2.y);
}

inline b2Vec2 ToB2Vec2(const Vector3& vector)
{
	return { vector._x, vector._y };
}

inline Vector3 ToVector3(const b2Vec2& vec2)
{
	return Vector3(vec2.x, vec2.y, 0.0f);
}

}