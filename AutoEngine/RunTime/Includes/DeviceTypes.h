#ifndef DEVICE_TYPES_H_
#define DEVICE_TYPES_H_

AUTO_BEGIN
struct TextureID
{
	explicit TextureID() : m_ID(0) { }
	explicit TextureID(unsigned int i) : m_ID(i) { }
	bool operator==(const TextureID& o) const { return m_ID == o.m_ID; }
	bool operator!=(const TextureID& o) const { return m_ID != o.m_ID; }
	bool operator < (const TextureID& o) const { return m_ID<o.m_ID; }
	unsigned int m_ID;
};

enum TextureDimension
{
	kTexDimUnknown = -1, // unknown
	kTexDimNone = 0, // no texture
	kTexDimDeprecated1D, // not used anymore, value there for backwards compatibility in serialization
	kTexDim2D,
	kTexDim3D,
	kTexDimCUBE,
	kTexDimAny,
	kTexDimCount, // keep this last!
	kTexDimForce32Bit = 0x7fffffff
};
AUTO_END

#endif //!DEVICE_TYPES_H_