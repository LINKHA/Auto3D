#pragma once
#include "AutoConfig.h"
#include "Math/Color.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

#include <imgui.h>

namespace Auto3D
{

inline AUTO_API ImVec2 ToImVal(const TVector2F& vec)
{
	return ImVec2(vec._x, vec._y);
}

inline AUTO_API ImVec2* ToImVal(const TVector2F* vec)
{
	if (vec == NULL)
		return NULL;
	ImVec2* imVec = new ImVec2(vec->_x, vec->_y);
	delete vec;
	vec = nullptr;

	return imVec;
}

inline AUTO_API ImVec4 ToImVal(const TVector4F& vec)
{
	return ImVec4(vec._x, vec._y, vec._z, vec._w);
}

inline AUTO_API ImVec4* ToImVal(const TVector4F* vec)
{
	if (vec == NULL)
		return NULL;

	ImVec4* imVec = new ImVec4(vec->_x, vec->_y, vec->_z, vec->_w);
	delete vec;
	vec = nullptr;

	return imVec;
}


inline AUTO_API TVector2F ImToVal(const ImVec2& vec)
{
	return TVector2F(vec.x, vec.y);
}

inline AUTO_API TVector4F ImToVal(const ImVec4& vec)
{
	return TVector4F(vec.x, vec.y, vec.z, vec.w);
}


}