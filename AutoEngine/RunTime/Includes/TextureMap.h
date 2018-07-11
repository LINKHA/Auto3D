#ifndef TEXTURE_MAP_H_
#define TEXTURE_MAP_H_
#include "Texture.h"
#include "Texture.h"
#include "OpenGLGather.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
AUTO_BEGIN

class TextureMap : public Texture2D
{
	REGISTER_DERIVED_CLASS(TextureMap, Texture2D);
	DECLARE_OBJECT_SERIALIZE(TextureMap);

public:

	TextureMap();
	void Start()override;
	void Draw(Camera * cam)override;


	void SetTexParameters(const TexParams& params);

};

AUTO_END

#endif //!TEXTURE_MAP_H_