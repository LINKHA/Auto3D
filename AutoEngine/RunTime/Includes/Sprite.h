#pragma once
#include "Texture2D.h"

AUTO_BEGIN
class Image;
class Sprite : public Texture2D
{
	REGISTER_DERIVED_CLASS(Sprite, Texture2D);
	DECLARE_OBJECT_SERIALIZE(Sprite);
public:

	Sprite();
	Sprite(char* imagePath);
	Sprite(char* imagePath, const Shader& shader);

	void Start()override;
	void Draw()override;

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetLinerParameters();
	void SetNearestParameters();

	void SetTexParameters(const TexParams& params);
	void GenerateMipmap();
private:
	unsigned int _VBO, _VAO, _EBO;
	unsigned int _textureData;

	Shader _shader;
	Color _color;
	Ptr(char, _imagePath);
	Ptr(Image, _image);

	bool _isMipmaps;
};
AUTO_END
