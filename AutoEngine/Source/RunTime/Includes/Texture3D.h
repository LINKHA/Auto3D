#pragma once
#include "Texture.h"

namespace Auto3D {

class Texture3D : public Texture
{
	REGISTER_OBJECT_CLASS(Texture3D, Texture)
public:
	explicit Texture3D(Ambient* ambient);
	void Start()override;
	void Draw()override;
};

}
