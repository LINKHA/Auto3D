#ifndef BASE_TEXTURE_H_
#define BASE_TEXTURE_H_
#include "GameObject.h"
#include "DeviceTypes.h"
#include "stl_use.h"
#include "Math/AUMath.h"
#include "OpenGLGather.h"
#include "stb_image.h"
#include "Shader.h"
#include "AtConfig.h"
#include "Motion.h"

AUTO_BEGIN

class Texture : public Motion
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Texture, Object);
	DECLARE_OBJECT_SERIALIZE(Texture);
	
protected:
	TextureID		m_TexID;
	int		        m_UsageMode;
	int				m_ColorSpace;
	float			m_UVScaleX, m_UVScaleY;
	float			m_TexelSizeX, m_TexelSizeY;


public:
	Texture();
	virtual void Start() {}
	virtual void Update() {}



};

AUTO_END

#endif // !BASE_TEXTURE_H_
