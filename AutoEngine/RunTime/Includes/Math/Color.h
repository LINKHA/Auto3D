#ifndef COLOR_H
#define COLOR_H
#include <algorithm>
#include "Math/FloatConversion.h"
#include "Vector3.h"
MATH_BEGIN
class Color
{
public:
	float	r, g, b, a;

	Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	Color(float inR, float inG, float inB, float inA = 1.0f) : r(inR), g(inG), b(inB), a(inA) {}
	explicit Color(const float* c) : r(c[0]), g(c[1]), b(c[2]), a(c[3]) {}
	explicit Color(const Vector3& c) : r(c.x), g(c.y), b(c.z), a(1.0f) {}
	template<class TransferFunction>
	void Transfer(TransferFunction& transfer);

	void Set(float inR, float inG, float inB, float inA)	{ r = inR; g = inG; b = inB; a = inA; }

	void SetHex(UInt32 hex)									{ Set(float(hex >> 24) / 255.0f, float((hex >> 16) & 255) / 255.0f, float((hex >> 8) & 255) / 255.0f, float(hex & 255) / 255.0f); }

	UInt32 GetHex() const									{ UInt32 hex = (NormalizedToByte(r) << 24) | (NormalizedToByte(g) << 16) | (NormalizedToByte(b) << 8) | NormalizedToByte(a); return hex; }

	float AverageRGB() const								{ return (r + g + b)*(1.0f / 3.0f); }
	float GreyScaleValue() const							{ return r * 0.30f + g * 0.59f + b * 0.11f; }

	Color& operator = (const Color& in)						{ Set(in.r, in.g, in.b, in.a); return *this; }

	bool Equals(const Color& inRGB) const					{ return(r == inRGB.r && g == inRGB.g && b == inRGB.b && a == inRGB.a);}

	bool NotEquals(const Color& inRGB) const				{ return (r != inRGB.r || g != inRGB.g || b != inRGB.b || a != inRGB.a); }

	float* GetPtr()											{ return &r; }
	const float* GetPtr() const								{ return &r; }

	Color& operator += (const Color &inRGBA)				{ r += inRGBA.r; g += inRGBA.g; b += inRGBA.b; a += inRGBA.a; return *this; }

	Color& operator *= (const Color &inRGBA)				{ r *= inRGBA.r; g *= inRGBA.g; b *= inRGBA.b; a *= inRGBA.a; return *this; }

private:
	// intentionally undefined
	bool operator == (const Color& inRGB) const;
	bool operator != (const Color& inRGB) const;
};


inline Color operator + (const Color& inC0, const Color& inC1)
{
	return Color(inC0.r + inC1.r, inC0.g + inC1.g, inC0.b + inC1.b, inC0.a + inC1.a);
}

inline Color operator * (const Color& inC0, const Color& inC1)
{
	return Color(inC0.r * inC1.r, inC0.g * inC1.g, inC0.b * inC1.b, inC0.a * inC1.a);
}

inline Color operator * (float inScale, const Color& inC0)
{
	return Color(inC0.r * inScale, inC0.g * inScale, inC0.b * inScale, inC0.a * inScale);
}

inline Color operator * (const Color& inC0, float inScale)
{
	return Color(inC0.r * inScale, inC0.g * inScale, inC0.b * inScale, inC0.a * inScale);
}

inline Color Lerp(const Color& c0, const Color& c1, float t)
{
	return (1.0f - t) * c0 + t * c1;
}

MATH_END
#endif

