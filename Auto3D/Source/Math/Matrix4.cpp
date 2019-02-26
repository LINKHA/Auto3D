#include "../Base/String.h"
#include "Matrix4.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const Matrix4 Matrix4::ZERO(
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f);

const Matrix4 Matrix4::IDENTITY(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

bool Matrix4::FromString(const String& str)
{
    return FromString(str.CString());
}

bool Matrix4::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 16)
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
    _m30 = (float)strtod(ptr, &ptr);
    _m31 = (float)strtod(ptr, &ptr);
    _m32 = (float)strtod(ptr, &ptr);
    _m33 = (float)strtod(ptr, &ptr);
    
    return true;
}

void Matrix4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
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

Matrix4 Matrix4::Inverse() const
{
    float v0 = _m20 * _m31 - _m21 * _m30;
    float v1 = _m20 * _m32 - _m22 * _m30;
    float v2 = _m20 * _m33 - _m23 * _m30;
    float v3 = _m21 * _m32 - _m22 * _m31;
    float v4 = _m21 * _m33 - _m23 * _m31;
    float v5 = _m22 * _m33 - _m23 * _m32;
    
    float i00 = (v5 * _m11 - v4 * _m12 + v3 * _m13);
    float i10 = -(v5 * _m10 - v2 * _m12 + v1 * _m13);
    float i20 = (v4 * _m10 - v2 * _m11 + v0 * _m13);
    float i30 = -(v3 * _m10 - v1 * _m11 + v0 * _m12);
    
    float invDet = 1.0f / (i00 * _m00 + i10 * _m01 + i20 * _m02 + i30 * _m03);
    
    i00 *= invDet;
    i10 *= invDet;
    i20 *= invDet;
    i30 *= invDet;
    
    float i01 = -(v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
    float i11 = (v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
    float i21 = -(v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
    float i31 = (v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;
    
    v0 = _m10 * _m31 - _m11 * _m30;
    v1 = _m10 * _m32 - _m12 * _m30;
    v2 = _m10 * _m33 - _m13 * _m30;
    v3 = _m11 * _m32 - _m12 * _m31;
    v4 = _m11 * _m33 - _m13 * _m31;
    v5 = _m12 * _m33 - _m13 * _m32;
    
    float i02 = (v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
    float i12 = -(v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
    float i22 = (v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
    float i32 = -(v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;
    
    v0 = _m21 * _m10 - _m20 * _m11;
    v1 = _m22 * _m10 - _m20 * _m12;
    v2 = _m23 * _m10 - _m20 * _m13;
    v3 = _m22 * _m11 - _m21 * _m12;
    v4 = _m23 * _m11 - _m21 * _m13;
    v5 = _m23 * _m12 - _m22 * _m13;
    
    float i03 = -(v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
    float i13 = (v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
    float i23 = -(v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
    float i33 = (v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;
    
    return Matrix4(
        i00, i01, i02, i03,
        i10, i11, i12, i13,
        i20, i21, i22, i23,
        i30, i31, i32, i33
    );
}

String Matrix4::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m03, _m10, _m11, _m12, _m13, _m20,
        _m21, _m22, _m23, _m30, _m31, _m32, _m33);
    return String(tempBuffer);
}

}
