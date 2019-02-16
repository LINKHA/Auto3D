#pragma once
#include "tTexture2D.h"
#include "AutoOGL.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"

namespace Auto3D {

class tTextureMap : public tTexture2D
{
	REGISTER_OBJECT_ABSTRACT_CLASS(tTextureMap, tTexture2D)
public:
	explicit tTextureMap(SharedPtr<Ambient> ambient);
	void Start()override;
	void Draw()override;


	void SetTexParameters(const TexParams& params);

};

}
