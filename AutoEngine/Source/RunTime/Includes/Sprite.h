#pragma once
#include "Texture2D.h"

namespace Auto3D {
class tImage;
class Image;
class Sprite : public Texture2D
{
	REGISTER_DERIVED_CLASS(Sprite, Texture2D);
	DECLARE_OBJECT_SERIALIZE(Sprite);
public:

	explicit Sprite(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);



	void Start()override;
	void Draw()override;
	/**
* @brief : Set image path to load image
*/
	void SettImage(char* imagePath);

	void SetImage(Image* image);

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetLinerParameters();
	void SetNearestParameters();

	void SetTexParameters(const TexParams& params);

	Image* GetImage() const { return _image.get(); }


	void GenerateMipmap();


private:
	unsigned int _VBO, _VAO, _EBO;
	unsigned int _textureData;

	Shader _shader;
	Color _color;

	char* _imagePath;
	tImage* _timage;
	SharedPtr<Image> _image;
	bool _isMipmaps;
};
}
