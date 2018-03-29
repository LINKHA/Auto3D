#ifndef BASE_TEXTURE_H_
#define BASE_TEXTURE_H_
#include "BaseObject.h"
#include "DeviceTypes.h"
#include "stl_use.h"

AUTO_BEGIN

class BaseTexture : public Object
{
protected:
	//TextureID		m_TexID;
	int		        m_UsageMode;
	int				m_ColorSpace;
	// Used by movie textures so that 0..1 range covers only the
	// movie portion. For all other textures this is (1,1).
	float			m_UVScaleX, m_UVScaleY;
	// Texel size. This is 1/size for all textures.
	float			m_TexelSizeX, m_TexelSizeY;

	//???
	//_VECTOR(ShaderLab::TexEnv*) m_TexEnvUsers;

public:
	BaseTexture();

	virtual bool MainThreadCleanup();

	virtual void Reset();

	virtual void CheckConsistency();

	virtual TextureDimension GetDimension() const = 0;

	//virtual bool ExtractImage(ImageReference* image, int imageIndex = 0) const = 0;

};

AUTO_END

#endif // !BASE_TEXTURE_H_
