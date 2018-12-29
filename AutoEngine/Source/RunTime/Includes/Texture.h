#pragma once
#include "DeviceTypes.h"
#include "Math/Math.h"
#include "AutoOGL.h"
#include "AutoImage.h"
#include "RenderComponent.h"
#include "GLTextureEnable.h"
namespace Auto3D {

class Texture : public RenderComponent, public GLTextureEnable
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Texture, RenderComponent)
protected:
	TextureID		_texID;
	int		        _usageMode;
	int				_colorSpace;
	float			_uVScaleX;
	float			_uVScaleY;
	float			_texelSizeX;
	float			_texelSizeY;
public:
	explicit Texture(SharedPtr<Ambient> ambient);
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}
};

}

