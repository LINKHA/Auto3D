#pragma once
#include "Texture2D.h"
#include "Singleton.h"
#include "stl_use.h"
AUTO_BEGIN
class Image;
class SpriteTranslucent : public Texture2D
{
	
public:

	SpriteTranslucent();
	~SpriteTranslucent();
	SpriteTranslucent(char* imagePath);
	SpriteTranslucent(char* imagePath, const Shader& shader);

	void Start()override;
	void Draw()override;
	void DrawTranslucentSprite();

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
class SpriteTranslucentManager : public Singleton<SpriteTranslucentManager>
{
private:
	std::vector<SpriteTranslucent*> _sprites;
	std::map<float, SpriteTranslucent*> _sorted;
public:
	void AddSprite(SpriteTranslucent * sprite);
	void ComputeMap();
	void RenderSprite();
};
AUTO_END
