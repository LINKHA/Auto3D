#include "../Base/String.h"
#include "Matrix3.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const Matrix3 Matrix3::ZERO(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f);

const Matrix3 Matrix3::IDENTITY(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f);

bool Matrix3::FromString(const String& str)
{
    return FromString(str.CString());
}

bool Matrix3::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 9)
        return false;
    
    char* ptr = (char*)str;
    _m00 = (float)strtod(ptr, &ptr);
    _m01 = (float)strtod(ptr, &ptr);
    _m02 = (float)strtod(ptr, &ptr);
    _m10 = (float)strtod(ptr, &ptr);
    _m11 = (float)strtod(ptr, &ptr);
    _m12 = (float)strtod(ptr, &ptr);
    _m20 = (float)strtod(ptr, &ptr);
    _m21 = (float)strtod(ptr, &ptr);
    _m22 = (float)strtod(ptr, &ptr);
    
    return true;
}

Matrix3 Matrix3::Inverse() const
{
    float det = _m00 * _m11 * _m22 +
        _m10 * _m21 * _m02 +
        _m20 * _m01 * _m12 -
        _m20 * _m11 * _m02 -
        _m10 * _m01 * _m22 -
        _m00 * _m21 * _m12;
    
    float invDet = 1.0f / det;
    
    return Matrix3(
        (_m11 * _m22 - _m21 * _m12) * invDet,
        -(_m01 * _m22 - _m21 * _m02) * invDet,
        (_m01 * _m12 - _m11 * _m02) * invDet,
        -(_m10 * _m22 - _m20 * _m12) * invDet,
        (_m00 * _m22 - _m20 * _m02) * invDet,
        -(_m00 * _m12 - _m10 * _m02) * invDet,
        (_m10 * _m21 - _m20 * _m11) * invDet,
        -(_m00 * _m21 - _m20 * _m01) * invDet,
        (_m00 * _m11 - _m10 * _m01) * invDet
    );
}

String Matrix3::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m10, _m11, _m12, _m20, _m21, _m22);
    return String(tempBuffer);
}

}
