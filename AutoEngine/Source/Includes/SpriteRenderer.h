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
	/**
	* @brief : Construct
	*/
	explicit SpriteRenderer(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/** 
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Override Draw
	*/
	void Draw()override;
	/**
	* @brief : Destory sprite renderer
	*/
	void Destory()override;
	/**
	* @brief : Set image to load image
	*/
	void SetImage(SharedPtr<Image> image);
	/**
	* @brief : Set color with Color
	*/
	void SetColor(const Color& color);
	/**
	* @brief : Set color with Vector3
	*/
	void SetColor(const Vector3& vec);
	/**
	* @brief : Set color with float r,g,b,a
	*/
	void SetColor(float r, float g, float b, float a = 1.0f);
	/**
	* @brief : Set shader
	*/
	void SetShader(SharedPtr<Shader> shader);
	/**
	* @brief : Set liner parameters
	*/
	void SetLinerParameters();
	/**
	* @brief : Set nearest parameters
	*/
	void SetNearestParameters();
	/**
	* @brief : Set texture parameters
	*/
	void SetTexParameters(const TexParams& params);
	/**
	* @brief : Get image 
	*/
	SharedPtr<Image> GetImage() { return _image; }
	/**
	* @brief : Get generate mipmap
	*/
	void GenerateMipmap();
private:
	unsigned _VBO;
	unsigned _EBO;
	unsigned int _textureData;
	Color _color;
	SharedPtr<Image> _image;
	SharedPtr<ShaderVariation> _shader;
	bool _isMipmaps;
};
}
