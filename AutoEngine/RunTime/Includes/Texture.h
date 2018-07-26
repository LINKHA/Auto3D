#pragma once
#include "GameObject.h"
#include "DeviceTypes.h"
#include "stl_use.h"
#include "Math/Math.h"
#include "OpenGLGather.h"
#include "stb_image.h"
#include "Shader.h"
#include "AtConfig.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "GLTextureEnable.h"
AUTO_BEGIN

class Texture : public RenderComponent, public GLTextureEnable
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Texture, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(Texture);
	
protected:
	TextureID		_texID;
	int		        _usageMode;
	int				_colorSpace;
	float			_uVScaleX;
	float			_uVScaleY;
	float			_texelSizeX;
	float			_texelSizeY;
public:
	Texture();
	explicit Texture(Ambient* ambient);
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}
};

AUTO_END

