#pragma once
#include "Texture2D.h"

namespace Auto3D {
class Image;
class Shader;
class ShaderVariation;

class SpriteRenderer : public Texture2D , public EnableSharedFromThis<SpriteRenderer>
{
	REGISTER_OBJECT_CLASS(SpriteRenderer, Texture2D)
public:
	explicit SpriteRenderer(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);

	void Start()override;
	void Draw()override;

	void Destory()override;
	/**
	* @brief : Set image to load image
	*/
	void SetImage(SharedPtr<Image> image);

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetShader(SharedPtr<Shader> shader);

	void SetLinerParameters();
	void SetNearestParameters();

	void SetTexParameters(const TexParams& params);

	SharedPtr<Image> GetImage() { return _image; }


	void GenerateMipmap();

private:
	unsigned int _VBO, _VAO, _EBO;
	unsigned int _textureData;

	Color _color;

	SharedPtr<Image> _image;

	SharedPtr<ShaderVariation> _shader;
	bool _isMipmaps;
};
}
