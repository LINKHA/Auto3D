#ifndef FLOAT_CONVERSION_H_
#define FLOAT_CONVERSION_H_

#include "Math/AUMathBase.h"
#include "LogAssert.h"
MATH_BEGIN
inline UInt32 FloorfToIntPos(float f)
{
	DebugAssertIf(f < 0 || f > UINT_MAX);
	return (UInt32)f;
}

inline UInt32 RoundfToIntPos(float f)
{
	return FloorfToIntPos(f + 0.5f);
}



inline int NormalizedToByte(float f)
{
	f = max(f, 0.0f);
	f = min(f, 1.0f);
	return RoundfToIntPos(f * 255.0f);
}

MATH_END

#endif // !FLOAT_CONVERSION_H_
