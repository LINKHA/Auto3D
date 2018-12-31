#pragma once
#include "Math/MathBase.h"

namespace Auto3D {
inline unsigned FloorfToIntPos(float f)
{
	Assert(f >= 0 && f <= UINT_MAX);
	return (unsigned)f;
}
inline unsigned RoundfToIntPos(float f)
{
	return FloorfToIntPos(f + 0.5f);
}

inline int NormalizedToByte(float f)
{
	f = max(f, 0.0f);
	f = min(f, 1.0f);
	return RoundfToIntPos(f * 255.0f);
}

}

