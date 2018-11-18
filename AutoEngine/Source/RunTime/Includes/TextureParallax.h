#pragma once
#include "Texture.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "TextureMap.h"
namespace Auto3D {

class TextureParallax : public TextureMap
{
	REGISTER_DERIVED_CLASS(TextureParallax, TextureMap);
	DECLARE_OBJECT_SERIALIZE(TextureParallax);
public:
	explicit TextureParallax(Ambient* ambient);
	//TextureParallax(char* imagePath);
	//TextureParallax(char* imagePath, const Shader& shader);
	void Start()override;
	void Draw()override;

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
private:

	unsigned int _VBO, _VAO;
	unsigned int _textureData;

	Shader _shader;
	Color _color;
	char* _imagePath;
	char* _imageNormalPath;
	char* _imageParallaxPath;
	unsigned int _image;
	unsigned int _imageNormal;
	unsigned int _imageParallax;

	void renderQuad();
};

}
