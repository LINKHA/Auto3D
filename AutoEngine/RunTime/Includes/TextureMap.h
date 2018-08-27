#pragma once
#include "Texture2D.h"
#include "GLGather.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
namespace Auto3D {

class TextureMap : public Texture2D
{
	REGISTER_DERIVED_CLASS(TextureMap, Texture2D);
	DECLARE_OBJECT_SERIALIZE(TextureMap);

public:
	TextureMap();
	explicit TextureMap(Ambient* ambient);
	void Start()override;
	void Draw()override;


	void SetTexParameters(const TexParams& params);

};

}
