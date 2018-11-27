#pragma once
#include "Texture2D.h"
#include "Singleton.h"

namespace Auto3D {

class tImage;
class SpriteTranslucent : public Texture2D
{
	
public:

	SpriteTranslucent(Ambient* ambient,char* imagePath);
	//SpriteTranslucent(char* imagePath, const Shader& shader);
	~SpriteTranslucent();
	void Start()override;
	void DrawTranslucent()override;

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
	char* _imagePath;
	tImage* _image;


	bool _isMipmaps;
};
}
