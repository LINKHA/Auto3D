#pragma once
#include "Texture.h"

namespace Auto3D {

class Texture3D : public Texture
{
	REGISTER_DERIVED_CLASS(Texture3D, Texture);
	DECLARE_OBJECT_SERIALIZE(Texture3D);
public:
	//Texture3D();
	explicit Texture3D(Ambient* ambient);
	void Start()override;
	void Draw()override;
};

}
