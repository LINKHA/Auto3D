#pragma once
#include "DeviceTypes.h"
#include "AutoMath.h"
#include "AutoOGL.h"
#include "AutoImage.h"
#include "RenderComponent.h"
#include "GLTextureEnable.h"
namespace Auto3D {

class tTexture : public RenderComponent, public GLTextureEnable
{
	REGISTER_OBJECT_ABSTRACT_CLASS(tTexture, RenderComponent)
protected:
	TextureID		_texID;
	int		        _usageMode;
	int				_colorSpace;
	float			_uVScaleX;
	float			_uVScaleY;
	float			_texelSizeX;
	float			_texelSizeY;
public:
	explicit tTexture(SharedPtr<Ambient> ambient);
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}
};

}

