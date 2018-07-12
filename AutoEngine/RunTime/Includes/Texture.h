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
#include "GameObject.h"
#include "RanderComponent.h"
#include "GLTextureEnable.h"
AUTO_BEGIN

class Texture : public RanderComponent , public GLTextureEnable
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Texture, RanderComponent);
	DECLARE_OBJECT_SERIALIZE(Texture);
	
protected:
	TextureID		m_TexID;
	int		        m_UsageMode;
	int				m_ColorSpace;
	float			m_UVScaleX, m_UVScaleY;
	float			m_TexelSizeX, m_TexelSizeY;
public:
	Texture();
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw(Camera* camera = nullptr) {}
};

AUTO_END

#endif // !BASE_TEXTURE_H_
