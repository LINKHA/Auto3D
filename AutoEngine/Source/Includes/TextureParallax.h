#pragma once
#include "Texture.h"
#include "Texture.h"
#include "Transform.h"
#include "tCamera.h"
#include "Application.h"
#include "tTextureMap.h"

namespace Auto3D {

class TextureParallax : public tTextureMap
{
	REGISTER_OBJECT_CLASS(TextureParallax, tTextureMap)
public:
	explicit TextureParallax(SharedPtr<Ambient> ambient);

	void Destory()override;

	void Start()override;
	void Draw()override;

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
private:

	unsigned int _VBO;
	unsigned int _textureData;

	SharedPtr<ShaderVariation> _shader;
	Color _color;
	char* _imagePath;
	char* _imageNormalPath;
	char* _imageParallaxPath;
	unsigned int _timage;
	unsigned int _imageNormal;
	unsigned int _imageParallax;

};

}
