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
inline AUTO_API btVector3 ToBtVector3(const TVector3F& vector)
{
	return btVector3(vector._x, vector._y, vector._z);
}

inline AUTO_API btQuaternion ToBtQuaternion(const FQuaternion& quaternion)
{
	return btQuaternion(quaternion._x, quaternion._y, quaternion._z, quaternion._w);
}

inline AUTO_API TVector3F BtToVector3(const btVector3& vector)
{
	return TVector3F(vector.x(), vector.y(), vector.z());
}

inline AUTO_API FQuaternion BtToQuaternion(const btQuaternion& quaternion)
{
	return FQuaternion(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}

inline AUTO_API bool BtHasWorldScaleChanged(const TVector3F& oldWorldScale, const TVector3F& newWorldScale)
{
	TVector3F delta = newWorldScale - oldWorldScale;
	float dot = Dot(delta, delta);
	return dot > 0.01f;
}



//Box2D
inline AUTO_API FColor B2ToColor(const b2Color& color)
{
	return FColor(color.r, color.g, color.b);
}

inline AUTO_API b2Vec2 ToB2Vector2(const TVector2F& vector)
{
	return { vector._x, vector._y };
}

inline AUTO_API TVector2F B2ToVector2(const b2Vec2& vec2)
{
	return TVector2F(vec2.x, vec2.y);
}

inline AUTO_API b2Vec2 ToB2Vector2(const TVector3F& vector)
{
	return { vector._x, vector._y };
}

inline AUTO_API TVector3F B2ToVector3(const b2Vec2& vec2)
{
	return TVector3F(vec2.x, vec2.y, 0.0f);
}

}