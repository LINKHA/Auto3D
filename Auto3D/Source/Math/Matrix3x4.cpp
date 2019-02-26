#include "../Base/String.h"
#include "Matrix3x4.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const Matrix3x4 Matrix3x4::ZERO(
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f);

const Matrix3x4 Matrix3x4::IDENTITY(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f);

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, float scale)
{
    SetRotation(rotation.RotationMatrix() * scale);
    SetTranslation(translation);
}

Matrix3x4::Matrix3x4(const Vector3& translation, const Quaternion& rotation, const Vector3& scale)
{
    SetRotation(rotation.RotationMatrix().Scaled(scale));
    SetTranslation(translation);
}

bool Matrix3x4::FromString(const String& str)
{
    return FromString(str.CString());
}

bool Matrix3x4::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 12)
        return false;
    
    char* ptr = (char*)str;
    _m00 = (float)strtod(ptr, &ptr);
    _m01 = (float)strtod(ptr, &ptr);
    _m02 = (float)strtod(ptr, &ptr);
    _m03 = (float)strtod(ptr, &ptr);
    _m10 = (float)strtod(ptr, &ptr);
    _m11 = (float)strtod(ptr, &ptr);
    _m12 = (float)strtod(ptr, &ptr);
    _m13 = (float)strtod(ptr, &ptr);
    _m20 = (float)strtod(ptr, &ptr);
    _m21 = (float)strtod(ptr, &ptr);
    _m22 = (float)strtod(ptr, &ptr);
    _m23 = (float)strtod(ptr, &ptr);
    
    return true;
}

void Matrix3x4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
{
    translation._x = _m03;
    translation._y = _m13;
    translation._z = _m23;
    
    scale._x = sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20);
    scale._y = sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21);
    scale._z = sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22);

    Vector3 invScale(1.0f / scale._x, 1.0f / scale._y, 1.0f / scale._z);
    rotation = Quaternion(ToMatrix3().Scaled(invScale));
}

Matrix3x4 Matrix3x4::Inverse() const
{
    float det = _m00 * _m11 * _m22 +
        _m10 * _m21 * _m02 +
        _m20 * _m01 * _m12 -
        _m20 * _m11 * _m02 -
        _m10 * _m01 * _m22 -
        _m00 * _m21 * _m12;
    
    float invDet = 1.0f / det;
    Matrix3x4 ret;
    
    ret._m00 = (_m11 * _m22 - _m21 * _m12) * invDet;
    ret._m01 = -(_m01 * _m22 - _m21 * _m02) * invDet;
    ret._m02 = (_m01 * _m12 - _m11 * _m02) * invDet;
    ret._m03 = -(_m03 * ret._m00 + _m13 * ret._m01 + _m23 * ret._m02);
    ret._m10 = -(_m10 * _m22 - _m20 * _m12) * invDet;
    ret._m11 = (_m00 * _m22 - _m20 * _m02) * invDet;
    ret._m12 = -(_m00 * _m12 - _m10 * _m02) * invDet;
    ret._m13 = -(_m03 * ret._m10 + _m13 * ret._m11 + _m23 * ret._m12);
    ret._m20 = (_m10 * _m21 - _m20 * _m11) * invDet;
    ret._m21 = -(_m00 * _m21 - _m20 * _m01) * invDet;
    ret._m22 = (_m00 * _m11 - _m10 * _m01) * invDet;
    ret._m23 = -(_m03 * ret._m20 + _m13 * ret._m21 + _m23 * ret._m22);
    
    return ret;
}

String Matrix3x4::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m03, _m10, _m11, _m12, _m13, _m20, _m21, _m22,
        _m23);
    return String(tempBuffer);
}

}
