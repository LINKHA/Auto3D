//#ifndef COLOR_H
//#define COLOR_H
//
//
//#include <algorithm>
//
//
//class ColorRGBAf
//{
//public:
//	float	r, g, b, a;
//
//	DEFINE_GET_TYPESTRING_IS_ANIMATION_CHANNEL(ColorRGBA)
//
//		ColorRGBAf() {}
//
//	ColorRGBAf(float inR, float inG, float inB, float inA = 1.0F) : r(inR), g(inG), b(inB), a(inA) {}
//	explicit ColorRGBAf(const float* c) : r(c[0]), g(c[1]), b(c[2]), a(c[3]) {}
//
//	template<class TransferFunction>
//	void Transfer(TransferFunction& transfer);
//
//	void Set(float inR, float inG, float inB, float inA) { r = inR; g = inG; b = inB; a = inA; }
//
//	void SetHex(UInt32 hex)
//	{
//		Set(float(hex >> 24) / 255.0f,
//			float((hex >> 16) & 255) / 255.0f,
//			float((hex >> 8) & 255) / 255.0f,
//			float(hex & 255) / 255.0f);
//	}
//
//	UInt32 GetHex() const
//	{
//		UInt32 hex = (NormalizedToByte(r) << 24) | (NormalizedToByte(g) << 16) | (NormalizedToByte(b) << 8) | NormalizedToByte(a);
//		return hex;
//	}
//
//	float AverageRGB() const { return (r + g + b)*(1.0F / 3.0F); }
//	float GreyScaleValue() const { return r * 0.30f + g * 0.59f + b * 0.11f; }
//
//	ColorRGBAf& operator = (const ColorRGBAf& in) { Set(in.r, in.g, in.b, in.a); return *this; }
//
//	bool Equals(const ColorRGBAf& inRGB) const
//	{
//		return (r == inRGB.r && g == inRGB.g && b == inRGB.b && a == inRGB.a);
//	}
//
//	bool NotEquals(const ColorRGBAf& inRGB) const
//	{
//		return (r != inRGB.r || g != inRGB.g || b != inRGB.b || a != inRGB.a);
//	}
//
//	float* GetPtr() { return &r; }
//	const float* GetPtr() const { return &r; }
//
//	ColorRGBAf& operator += (const ColorRGBAf &inRGBA)
//	{
//		r += inRGBA.r; g += inRGBA.g; b += inRGBA.b; a += inRGBA.a;
//		return *this;
//	}
//
//	ColorRGBAf& operator *= (const ColorRGBAf &inRGBA)
//	{
//		r *= inRGBA.r; g *= inRGBA.g; b *= inRGBA.b; a *= inRGBA.a;
//		return *this;
//	}
//
//private:
//	// intentionally undefined
//	bool operator == (const ColorRGBAf& inRGB) const;
//	bool operator != (const ColorRGBAf& inRGB) const;
//};
//
//
//inline ColorRGBAf operator + (const ColorRGBAf& inC0, const ColorRGBAf& inC1)
//{
//	return ColorRGBAf(inC0.r + inC1.r, inC0.g + inC1.g, inC0.b + inC1.b, inC0.a + inC1.a);
//}
//
//inline ColorRGBAf operator * (const ColorRGBAf& inC0, const ColorRGBAf& inC1)
//{
//	return ColorRGBAf(inC0.r * inC1.r, inC0.g * inC1.g, inC0.b * inC1.b, inC0.a * inC1.a);
//}
//
//inline ColorRGBAf operator * (float inScale, const ColorRGBAf& inC0)
//{
//	return ColorRGBAf(inC0.r * inScale, inC0.g * inScale, inC0.b * inScale, inC0.a * inScale);
//}
//
//inline ColorRGBAf operator * (const ColorRGBAf& inC0, float inScale)
//{
//	return ColorRGBAf(inC0.r * inScale, inC0.g * inScale, inC0.b * inScale, inC0.a * inScale);
//}
//
//inline ColorRGBAf Lerp(const ColorRGBAf& c0, const ColorRGBAf& c1, float t)
//{
//	return (1.0f - t) * c0 + t * c1;
//}
//
//
//
//class ColorRGBA32
//{
//public:
//
//	UInt8	r, g, b, a;
//
//	DEFINE_GET_TYPESTRING_IS_ANIMATION_CHANNEL(ColorRGBA)
//
//		ColorRGBA32() {}
//
//	ColorRGBA32(UInt8 inR, UInt8 inG, UInt8 inB, UInt8 inA) { r = inR; g = inG; b = inB; a = inA; }
//	ColorRGBA32(UInt32 c) { *(UInt32*)this = c; }
//	void Set(UInt8 inR, UInt8 inG, UInt8 inB, UInt8 inA) { r = inR; g = inG; b = inB; a = inA; }
//
//	ColorRGBA32 operator = (const ColorRGBA32& c) { *(UInt32*)this = *((UInt32*)&c); return *this; }
//
//	ColorRGBA32(const ColorRGBAf& c) { Set(c); }
//
//	operator ColorRGBAf() const
//	{
//		return ColorRGBAf(ByteToNormalized(r), ByteToNormalized(g), ByteToNormalized(b), ByteToNormalized(a));
//	}
//
//	UInt32 AsUInt32() const { return *(UInt32*)this; }
//
//	void operator = (const ColorRGBAf& c)
//	{
//		Set(c);
//	}
//
//	UInt32 GetUInt32() { return *(UInt32*)this; }
//
//	void Set(const ColorRGBAf& c)
//	{
//		r = NormalizedToByte(c.r);
//		g = NormalizedToByte(c.g);
//		b = NormalizedToByte(c.b);
//		a = NormalizedToByte(c.a);
//	}
//
//	template<class TransferFunction>
//	void Transfer(TransferFunction& transfer)
//	{
//		transfer.SetVersion(2);
//		UInt32* c = reinterpret_cast<UInt32*> (this);
//		// When transferring colors we shouldn't swap bytes.
//		// UInt32 already convert endianess by default so we convert it two times to keep it the same :)
//		if (transfer.ConvertEndianess())
//		{
//			if (transfer.IsReading())
//			{
//				transfer.Transfer(*c, "rgba", kHideInEditorMask);
//				SwapEndianBytes(*c);
//			}
//			else
//			{
//				UInt32 temp = *c;
//				SwapEndianBytes(temp);
//				transfer.Transfer(temp, "rgba", kHideInEditorMask);
//			}
//		}
//		else
//		{
//			transfer.Transfer(*c, "rgba", kHideInEditorMask);
//		}
//	}
//
//	UInt8& operator [] (long i) { return GetPtr()[i]; }
//	const UInt8& operator [] (long i)const { return GetPtr()[i]; }
//
//	bool operator == (const ColorRGBA32& inRGB) const
//	{
//		return (r == inRGB.r && g == inRGB.g && b == inRGB.b && a == inRGB.a) ? true : false;
//	}
//
//	bool operator != (const ColorRGBA32& inRGB) const
//	{
//		return (r != inRGB.r || g != inRGB.g || b != inRGB.b || a != inRGB.a) ? true : false;
//	}
//
//	UInt8* GetPtr() { return &r; }
//	const UInt8* GetPtr()const { return &r; }
//
//	inline ColorRGBA32 operator * (int scale) const
//	{
//		//AssertIf (scale < 0 || scale > 255);
//		scale += 1;
//		const UInt32& u = reinterpret_cast<const UInt32&> (*this);
//		UInt32 lsb = (((u & 0x00ff00ff) * scale) >> 8) & 0x00ff00ff;
//		UInt32 msb = (((u & 0xff00ff00) >> 8) * scale) & 0xff00ff00;
//		lsb |= msb;
//		return ColorRGBA32(lsb);
//	}
//
//	inline void operator *= (const ColorRGBA32& inC1)
//	{
//#if 0
//		r = (r * inC1.r) / 255;
//		g = (g * inC1.g) / 255;
//		b = (b * inC1.b) / 255;
//		a = (a * inC1.a) / 255;
//#else // This is much faster, but doesn't guarantee 100% matching result (basically color values van vary 1/255 but not at ends, check out unit test in cpp file).
//		UInt32& u = reinterpret_cast<UInt32&> (*this);
//		const UInt32& v = reinterpret_cast<const UInt32&> (inC1);
//		UInt32 result = (((u & 0x000000ff) * ((v & 0x000000ff) + 1)) >> 8) & 0x000000ff;
//		result |= (((u & 0x0000ff00) >> 8) * (((v & 0x0000ff00) >> 8) + 1)) & 0x0000ff00;
//		result |= (((u & 0x00ff0000) * (((v & 0x00ff0000) >> 16) + 1)) >> 8) & 0x00ff0000;
//		result |= (((u & 0xff000000) >> 8) * (((v & 0xff000000) >> 24) + 1)) & 0xff000000;
//		u = result;
//#endif
//	}
//
//	inline ColorRGBA32 SwizzleToBGRA() const { return ColorRGBA32(b, g, r, a); }
//	inline ColorRGBA32 SwizzleToBGR() const { return ColorRGBA32(b, g, r, 255); }
//	inline ColorRGBA32 SwizzleToARGB() const { return ColorRGBA32(a, r, g, b); }
//	inline ColorRGBA32 UnswizzleBGRA() const { return ColorRGBA32(b, g, r, a); }
//	inline ColorRGBA32 UnswizzleARGB() const { return ColorRGBA32(g, b, a, r); }
//};
//
//#if GFX_OPENGLESxx_ONLY
//inline ColorRGBA32 SwizzleColorForPlatform(const ColorRGBA32& col) { return col; }
//inline ColorRGBA32 UnswizzleColorForPlatform(const ColorRGBA32& col) { return col; }
//#elif UNITY_XENON || UNITY_PS3 || UNITY_WII
//inline ColorRGBA32 SwizzleColorForPlatform(const ColorRGBA32& col) { return col.SwizzleToARGB(); }
//inline ColorRGBA32 UnswizzleColorForPlatform(const ColorRGBA32& col) { return col.UnswizzleARGB(); }
//#else
//inline ColorRGBA32 SwizzleColorForPlatform(const ColorRGBA32& col) { return col.SwizzleToBGRA(); }
//inline ColorRGBA32 UnswizzleColorForPlatform(const ColorRGBA32& col) { return col.UnswizzleBGRA(); }
//#endif
//
//struct OpColorRGBA32ToUInt32
//{
//	typedef UInt32 result_type;
//	UInt32 operator() (ColorRGBA32 const& arg) const { return arg.AsUInt32(); }
//};
//
//inline ColorRGBA32 operator + (const ColorRGBA32& inC0, const ColorRGBA32& inC1)
//{
//	return ColorRGBA32(std::min<int>(inC0.r + inC1.r, 255),
//		std::min<int>(inC0.g + inC1.g, 255),
//		std::min<int>(inC0.b + inC1.b, 255),
//		std::min<int>(inC0.a + inC1.a, 255));
//}
//
//inline ColorRGBA32 operator * (const ColorRGBA32& inC0, const ColorRGBA32& inC1)
//{
//#if 0
//	return ColorRGBA32((inC0.r * inC1.r) / 255,
//		(inC0.g * inC1.g) / 255,
//		(inC0.b * inC1.b) / 255,
//		(inC0.a * inC1.a) / 255);
//#else
//	// This is much faster, but doesn't guarantee 100% matching result (basically color values van vary 1/255 but not at ends, check out unit test in cpp file).
//	const UInt32& u = reinterpret_cast<const UInt32&> (inC0);
//	const UInt32& v = reinterpret_cast<const UInt32&> (inC1);
//	UInt32 result = (((u & 0x000000ff) * ((v & 0x000000ff) + 1)) >> 8) & 0x000000ff;
//	result |= (((u & 0x0000ff00) >> 8) * (((v & 0x0000ff00) >> 8) + 1)) & 0x0000ff00;
//	result |= (((u & 0x00ff0000) * (((v & 0x00ff0000) >> 16) + 1)) >> 8) & 0x00ff0000;
//	result |= (((u & 0xff000000) >> 8) * (((v & 0xff000000) >> 24) + 1)) & 0xff000000;
//	return ColorRGBA32(result);
//#endif
//}
//
//inline ColorRGBA32 Lerp(const ColorRGBA32& c0, const ColorRGBA32& c1, int scale)
//{
//	//AssertIf (scale < 0 || scale > 255);
//	const UInt32& u0 = reinterpret_cast<const UInt32&> (c0);
//	const UInt32& u1 = reinterpret_cast<const UInt32&> (c1);
//	UInt32 vx = u0 & 0x00ff00ff;
//	UInt32 rb = vx + ((((u1 & 0x00ff00ff) - vx) * scale) >> 8) & 0x00ff00ff;
//	vx = u0 & 0xff00ff00;
//	return ColorRGBA32(rb | (vx + ((((u1 >> 8) & 0x00ff00ff) - (vx >> 8)) * scale) & 0xff00ff00));
//}
//
//
//template<class TransferFunction>
//void ColorRGBAf::Transfer(TransferFunction& transfer)
//{
//	transfer.AddMetaFlag(kTransferUsingFlowMappingStyle);
//	transfer.Transfer(r, "r", kHideInEditorMask);
//	transfer.Transfer(g, "g", kHideInEditorMask);
//	transfer.Transfer(b, "b", kHideInEditorMask);
//	transfer.Transfer(a, "a", kHideInEditorMask);
//}
//
//
//#endif
//
