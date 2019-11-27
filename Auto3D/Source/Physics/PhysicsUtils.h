#pragma once
#include "AutoConfig.h"
#include "Math/Color.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <Box2D.h>

namespace Auto3D 
{
//Bullet3
inline AUTO_API btVector3 ToBtVector3(const Vector3F& vector)
{
	return btVector3(vector._x, vector._y, vector._z);
}

inline AUTO_API btQuaternion ToBtQuaternion(const Quaternion& quaternion)
{
	return btQuaternion(quaternion._x, quaternion._y, quaternion._z, quaternion._w);
}

inline AUTO_API Vector3F BtToVector3(const btVector3& vector)
{
	return Vector3F(vector.x(), vector.y(), vector.z());
}

inline AUTO_API Quaternion BtToQuaternion(const btQuaternion& quaternion)
{
	return Quaternion(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}

inline AUTO_API bool BtHasWorldScaleChanged(const Vector3F& oldWorldScale, const Vector3F& newWorldScale)
{
	Vector3F delta = newWorldScale - oldWorldScale;
	float dot = Dot(delta, delta);
	return dot > 0.01f;
}



//Box2D
inline AUTO_API Color B2ToColor(const b2Color& color)
{
	return Color(color.r, color.g, color.b);
}

inline AUTO_API b2Vec2 ToB2Vector2(const Vector2F& vector)
{
	return { vector._x, vector._y };
}

inline AUTO_API Vector2F B2ToVector2(const b2Vec2& vec2)
{
	return Vector2F(vec2.x, vec2.y);
}

inline AUTO_API b2Vec2 ToB2Vector2(const Vector3F& vector)
{
	return { vector._x, vector._y };
}

inline AUTO_API Vector3F B2ToVector3(const b2Vec2& vec2)
{
	return Vector3F(vec2.x, vec2.y, 0.0f);
}

}