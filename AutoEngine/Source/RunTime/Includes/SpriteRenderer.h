#pragma once
#include "Texture2D.h"

namespace Auto3D {
class Image;
class Shader;
class ShaderVariation;

#define SpriteDebug 0

class SpriteRenderer : public Texture2D
{
	REGISTER_OBJECT_CLASS(SpriteRenderer, Texture2D)
public:
	explicit SpriteRenderer(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;
	void Draw()override;
	/**
	* @brief : Set image to load image
	*/
	void SetImage(Image* image);

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetLinerParameters();
	void SetNearestParameters();

	void SetTexParameters(const TexParams& params);

	Image* GetImage() { return _image.get(); }


	void GenerateMipmap();


private:
	unsigned int _VBO, _VAO, _EBO;
	unsigned int _textureData;

	_Shader _tshader;

	Color _color;

	SharedPtr<Shader> _shader;
	SharedPtr<Image> _image;

	SharedPtr<ShaderVariation> _shaderVar;
	bool _isMipmaps;
};
}
