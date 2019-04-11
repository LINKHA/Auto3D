#pragma once
#include "../AutoConfig.h"
#include "../Math/Color.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <Box2D.h>

namespace Auto3D 
{

inline AUTO_API btVector3 ToBtVector3(const Vector3F& vector);

inline AUTO_API btQuaternion ToBtQuaternion(const Quaternion& quaternion);

inline AUTO_API Vector3F ToVector3(const btVector3& vector);

inline AUTO_API Quaternion ToQuaternion(const btQuaternion& quaternion);

inline AUTO_API bool HasWorldScaleChanged(const Vector3F& oldWorldScale, const Vector3F& newWorldScale);

inline AUTO_API Color ToColor(const b2Color& color);

inline AUTO_API b2Vec2 ToB2Vec2(const Vector2F& vector);

inline AUTO_API Vector2F ToVector2(const b2Vec2& vec2);

inline AUTO_API b2Vec2 ToB2Vec2(const Vector3F& vector);

inline AUTO_API Vector3F ToVector3(const b2Vec2& vec2);

}