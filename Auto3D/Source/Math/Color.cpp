#include "Container/String.h"
#include "Color.h"

#include <cstdio>
#include <cstdlib>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const FColor FColor::WHITE(1.0f, 1.0f, 1.0f);
const FColor FColor::GRAY(0.5f, 0.5f, 0.5f);
const FColor FColor::BLACK(0.0f, 0.0f, 0.0f);
const FColor FColor::RED(1.0f, 0.0f, 0.0f);
const FColor FColor::GREEN(0.0f, 1.0f, 0.0f);
const FColor FColor::BLUE(0.0f, 0.0f, 1.0f);
const FColor FColor::CYAN(0.0f, 1.0f, 1.0f);
const FColor FColor::MAGENTA(1.0f, 0.0f, 1.0f);
const FColor FColor::YELLOW(1.0f, 1.0f, 0.0f);
const FColor FColor::TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);

unsigned FColor::ToUInt() const
{
    unsigned r = Clamp(((int)(_r * 255.0f)), 0, 255);
    unsigned g = Clamp(((int)(_g * 255.0f)), 0, 255);
    unsigned b = Clamp(((int)(_b * 255.0f)), 0, 255);
    unsigned a = Clamp(((int)(_a * 255.0f)), 0, 255);
    return (r << 24) | (g << 16) | (b << 8) | a;
}

TVector3F FColor::ToHSL() const
{
    float min, max;
    Bounds(&min, &max, true);

    float h = Hue(min, max);
    float s = SaturationHSL(min, max);
    float l = (max + min) * 0.5f;

    return TVector3F(h, s, l);
}

TVector3F FColor::ToHSV() const
{
    float min, max;
    Bounds(&min, &max, true);

    float h = Hue(min, max);
    float s = SaturationHSV(min, max);
    float v = max;

    return TVector3F(h, s, v);
}

void FColor::FromHSL(float h, float s, float l, float a_)
{
    float c;
    if (l < 0.5f)
        c = (1.0f + (2.0f * l - 1.0f)) * s;
    else
        c = (1.0f - (2.0f * l - 1.0f)) * s;

    float m = l - 0.5f * c;

    FromHCM(h, c, m);

    _a = a_;
}

void FColor::FromHSV(float h, float s, float v, float a_)
{
    float c = v * s;
    float m = v - c;

    FromHCM(h, c, m);

    _a = a_;
}

bool FColor::FromString(const FString& str)
{
    return FromString(str.CString());
}

bool FColor::FromString(const char* str)
{
    size_t elements = FString::CountElements(str, ' ');
    if (elements < 3)
        return false;
    
    char* ptr = (char*)str;
    _r = (float)strtod(ptr, &ptr);
    _g = (float)strtod(ptr, &ptr);
    _b = (float)strtod(ptr, &ptr);
    if (elements > 3)
        _a = (float)strtod(ptr, &ptr);
    
    return true;
}

float FColor::Chroma() const
{
    float min, max;
    Bounds(&min, &max, true);

    return max - min;
}

float FColor::Hue() const
{
    float min, max;
    Bounds(&min, &max, true);

    return Hue(min, max);
}

float FColor::SaturationHSL() const
{
    float min, max;
    Bounds(&min, &max, true);

    return SaturationHSL(min, max);
}

float FColor::SaturationHSV() const
{
    float min, max;
    Bounds(&min, &max, true);

    return SaturationHSV(min, max);
}

float FColor::Lightness() const
{
    float min, max;
    Bounds(&min, &max, true);

    return (max + min) * 0.5f;
}

void FColor::Bounds(float* min, float* max, bool clipped) const
{
    assert(min && max);

    if (_r > _g)
    {
        if (_g > _b) // r > g > b
        {
            *max = _r;
            *min = _b;
        }
        else // r > g && g <= b
        {
            *max = _r > _b ? _r : _b;
            *min = _g;
        }
    }
    else
    {
        if (_b > _g) // r <= g < b
        {
            *max = _b;
            *min = _r;
        }
        else // r <= g && b <= g
        {
            *max = _g;
            *min = _r < _b ? _r : _b;
        }
    }

    if (clipped)
    {
        *max = *max > 1.0f ? 1.0f : (*max < 0.0f ? 0.0f : *max);
        *min = *min > 1.0f ? 1.0f : (*min < 0.0f ? 0.0f : *min);
    }
}

float FColor::MaxRGB() const
{
    if (_r > _g)
        return (_r > _b) ? _r : _b;
    else
        return (_g > _b) ? _g : _b;
}

float FColor::MinRGB() const
{
    if (_r < _g)
        return (_r < _b) ? _r : _b;
    else
        return (_g < _b) ? _g : _b;
}

float FColor::Range() const
{
    float min, max;
    Bounds(&min, &max);
    return max - min;
}

void FColor::Clip(bool clipAlpha)
{
    _r = (_r > 1.0f) ? 1.0f : ((_r < 0.0f) ? 0.0f : _r);
    _g = (_g > 1.0f) ? 1.0f : ((_g < 0.0f) ? 0.0f : _g);
    _b = (_b > 1.0f) ? 1.0f : ((_b < 0.0f) ? 0.0f : _b);

    if (clipAlpha)
        _a = (_a > 1.0f) ? 1.0f : ((_a < 0.0f) ? 0.0f : _a);
}

void FColor::Invert(bool invertAlpha)
{
    _r = 1.0f - _r;
    _g = 1.0f - _g;
    _b = 1.0f - _b;

    if (invertAlpha)
        _a = 1.0f - _a;
}

FColor FColor::Lerp(const FColor &rhs, float t) const
{
    float invT = 1.0f - t;
    return FColor(
        _r * invT + rhs._r * t,
        _g * invT + rhs._g * t,
        _b * invT + rhs._b * t,
        _a * invT + rhs._a * t
    );
}

FString FColor::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g", _r, _g, _b, _a);
    return FString(tempBuffer);
}

float FColor::Hue(float min, float max) const
{
    float chroma = max - min;

    // If chroma equals zero, hue is undefined
    if (chroma <= M_EPSILON)
        return 0.0f;

    // Calculate and return hue
    if (Auto3D::Equals(_g, max))
        return (_b + 2.0f*chroma - _r) / (6.0f * chroma);
    else if (Auto3D::Equals(_b, max))
        return (4.0f * chroma - _g + _r) / (6.0f * chroma);
    else
    {
        float h = (_g - _b) / (6.0f * chroma);
        return (h < 0.0f) ? 1.0f + h : ((h >= 1.0f) ? h - 1.0f : h);
    }

}

float FColor::SaturationHSV(float min, float max) const
{
    // Avoid div-by-zero: result undefined
    if (max <= M_EPSILON)
        return 0.0f;

    // Saturation equals chroma:value ratio
    return 1.0f - (min / max);
}

float FColor::SaturationHSL(float min, float max) const
{
    // Avoid div-by-zero: result undefined
    if (max <= M_EPSILON || min >= 1.0f - M_EPSILON)
        return 0.0f;

    // Chroma = max - min, lightness = (max + min) * 0.5
    float hl = (max + min);
    if (hl <= 1.0f)
        return (max - min) / hl;
    else
        return (min - max) / (hl - 2.0f);

}

void FColor::FromHCM(float h, float c, float m)
{
    if (h < 0.0f || h >= 1.0f)
        h -= floorf(h);

    float hs = h * 6.0f;
    float x = c * (1.0f - Auto3D::Abs(fmodf(hs, 2.0f) - 1.0f));

    // Reconstruct r', g', b' from hue
    if (hs < 2.0f)
    {
        _b = 0.0f;
        if (hs < 1.0f)
        {
            _g = x;
            _r = c;
        }
        else
        {
            _g = c;
            _r = x;
        }
    }
    else if (hs < 4.0f)
    {
        _r = 0.0f;
        if (hs < 3.0f)
        {
            _g = c;
            _b = x;
        }
        else
        {
            _g = x;
            _b = c;
        }
    }
    else
    {
        _g = 0.0f;
        if (hs < 5.0f)
        {
            _r = x;
            _b = c;
        }
        else
        {
            _r = c;
            _b = x;
        }
    }

    _r += m;
    _g += m;
    _b += m;
}

}
