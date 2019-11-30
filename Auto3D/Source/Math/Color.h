#pragma once

#include "Vector4.h"

// Defined by Windows headers
#undef TRANSPARENT

namespace Auto3D
{

class FString;

/// RGBA color.
class AUTO_API FColor
{
public:
    /// Red value.
	union { float _r, _red; };
    /// Green value.
	union { float _g, _green; };
    /// Blue value.
	union { float _b, _blue; };
    /// Alpha value.
	union { float _a, _alpha; };

    /// Construct undefined.
    FColor()
    {
    }
    
    /// Copy-construct.
    FColor(const FColor& color) :
        _r(color._r),
        _g(color._g),
        _b(color._b),
        _a(color._a)
    {
    }
    
    /// Construct from another color and modify the alpha.
    FColor(const FColor& color, float a) :
        _r(color._r),
        _g(color._g),
        _b(color._b),
        _a(a)
    {
    }
    
    /// Construct from RGB values and set alpha fully opaque.
    FColor(float r, float g, float b) :
        _r(r),
        _g(g),
        _b(b),
        _a(1.0f)
    {
    }
    
    /// Construct from RGBA values.
    FColor(float r, float g, float b, float a) :
        _r(r),
        _g(g),
        _b(b),
        _a(a)
    {
    }

    /// Construct from a float array.
    FColor(const float* _data) :
        _r(_data[0]),
        _g(_data[1]),
        _b(_data[2]),
        _a(_data[3])
    {
    }
    
    /// Construct by parsing a string.
    FColor(const FString& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    FColor(const char* str)
    {
        FromString(str);
    }
    
    /// Add-assign a color.
    FColor& operator += (const FColor& rhs)
    {
        _r += rhs._r;
        _g += rhs._g;
        _b += rhs._b;
        _a += rhs._a;
        return *this;
    }
    
    /// Test for equality with another color without epsilon.
    bool operator == (const FColor& rhs) const { return _r == rhs._r && _g == rhs._g && _b == rhs._b && _a == rhs._a; }
    /// Test for inequality with another color without epsilon.
    bool operator != (const FColor& rhs) const { return !(*this == rhs); }
    /// Multiply with a scalar.
    FColor operator * (float rhs) const { return FColor(_r * rhs, _g * rhs, _b * rhs, _a * rhs); }
    /// Add a color.
    FColor operator + (const FColor& rhs) const { return FColor(_r + rhs._r, _g + rhs._g, _b + rhs._b, _a + rhs._a); }
    /// Substract a color.
    FColor operator - (const FColor& rhs) const { return FColor(_r - rhs._r, _g - rhs._g, _b - rhs._b, _a - rhs._a); }
    /// Return float data.
    const float* Data() const { return &_r; }
    /// Return color packed to a 32-bit integer, with R component in the lowest 8 bits. Components are clamped to [0, 1] range.
    unsigned ToUInt() const;
    /// Return HSL color-space representation as a TVector3; the RGB values are clipped before conversion but not changed in the process.
    TVector3F ToHSL() const;
    /// Return HSV color-space representation as a TVector3; the RGB values are clipped before conversion but not changed in the process.
    TVector3F ToHSV() const;
    /// Set RGBA values from specified HSL values and alpha.
    void FromHSL(float h, float s, float l, float a = 1.0f);
    /// Set RGBA values from specified HSV values and alpha.
    void FromHSV(float h, float s, float v, float a = 1.0f);
    /// Parse from a string. Return true on success.
    bool FromString(const FString& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);

    /// Return RGB as a three-dimensional vector.
    TVector3F ToVector3() const { return TVector3F(_r, _g, _b); }
    /// Return RGBA as a four-dimensional vector.
    TVector4F ToVector4() const { return TVector4F(_r, _g, _b, _a); }

    /// Return sum of RGB components.
    float SumRGB() const { return _r + _g + _b; }
    /// Return average value of the RGB channels.
    float Average() const { return (_r + _g + _b) / 3.0f; }

    /// Return the 'grayscale' representation of RGB values, as used by JPEG and PAL/NTSC among others.
    float Luma() const { return _r * 0.299f + _g * 0.587f + _b * 0.114f; }
    /// Return the colorfulness relative to the brightness of a similarly illuminated white.
    float Chroma() const;
    /// Return hue mapped to range [0, 1.0).
    float Hue() const;
    /// Return saturation as defined for HSL.
    float SaturationHSL() const;
    /// Return saturation as defined for HSV.
    float SaturationHSV() const;
    /// Return value as defined for HSV: largest value of the RGB components. Equivalent to calling MinRGB().
    float Value() const { return MaxRGB(); }
    /// Return lightness as defined for HSL: average of the largest and smallest values of the RGB components.
    float Lightness() const;

    /// Stores the values of least and greatest RGB component at specified pointer addresses, optionally clipping those values to [0, 1] range.
    void Bounds(float* min, float* max, bool clipped=false) const;
    /// Return the largest value of the RGB components.
    float MaxRGB() const;
    /// Return the smallest value of the RGB components.
    float MinRGB() const;
    /// Return range, defined as the difference between the greatest and least RGB component.
    float Range() const;

    /// Clip to [0, 1.0] range.
    void Clip(bool clipAlpha = false);
    /// Inverts the RGB channels and optionally the alpha channel as well.
    void Invert(bool invertAlpha = false);
    /// Return linear interpolation of this color with another color.
    FColor Lerp(const FColor& rhs, float t) const;
    /// Return color with absolute components.
    FColor Abs() const { return FColor(Auto3D::Abs(_r), Auto3D::Abs(_g), Auto3D::Abs(_b), Auto3D::Abs(_a)); }
    /// Test for equality with another color with epsilon.
    bool Equals(const FColor& rhs) const { return Auto3D::Equals(_r, rhs._r) && Auto3D::Equals(_g, rhs._g) && Auto3D::Equals(_b, rhs._b) && Auto3D::Equals(_a, rhs._a); }
    
    /// Return as string.
    FString ToString() const;
    
    /// Opaque white color.
    static const FColor WHITE;
    /// Opaque gray color.
    static const FColor GRAY;
    /// Opaque black color.
    static const FColor BLACK;
    /// Opaque red color.
    static const FColor RED;
    /// Opaque green color.
    static const FColor GREEN;
    /// Opaque blue color.
    static const FColor BLUE;
    /// Opaque cyan color.
    static const FColor CYAN;
    /// Opaque magenta color.
    static const FColor MAGENTA;
    /// Opaque yellow color.
    static const FColor YELLOW;
    /// Transparent color (black with no alpha).
    static const FColor TRANSPARENT;

protected:
    /// Return hue value given greatest and least RGB component, value-wise.
    float Hue(float min, float max) const;
    /// Return saturation (HSV) given greatest and least RGB component, value-wise.
    float SaturationHSV(float min, float max) const;
    /// Return saturation (HSL) given greatest and least RGB component, value-wise.
    float SaturationHSL(float min, float max) const;
    /// Calculate and set RGB values. Convenience function used by FromHSV and FromHSL to avoid code duplication.
    void FromHCM(float h, float c, float m);
};

/// Multiply FColor with a scalar.
inline FColor operator * (float lhs, const FColor& rhs) { return rhs * lhs; }

}
