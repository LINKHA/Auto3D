#pragma once
#include "Texture2D.h"
#include "AutoOGL.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"

namespace Auto3D {

class TextureMap : public Texture2D
{
	REGISTER_OBJECT_ABSTRACT_CLASS(TextureMap, Texture2D)
public:
	explicit TextureMap(SharedPtr<Ambient> ambient);
	void Start()override;
	void Draw()override;


	void SetTexParameters(const TexParams& params);

};

}
