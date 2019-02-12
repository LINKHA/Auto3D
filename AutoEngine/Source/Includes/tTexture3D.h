#pragma once
#include "tTexture.h"

namespace Auto3D {

class tTexture3D : public tTexture
{
	REGISTER_OBJECT_CLASS(tTexture3D, tTexture)
public:
	explicit tTexture3D(SharedPtr<Ambient> ambient);
	void Start()override;
	void Draw()override;
};

}
