#ifndef TEXTURE_PARALLAX_H_
#define TEXTURE_PARALLAX_H_
#include "Texture.h"
#include "Texture.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "TextureMap.h"
AUTO_BEGIN

class TextureParallax : public TextureMap
{
	REGISTER_DERIVED_CLASS(TextureParallax, TextureMap);
	DECLARE_OBJECT_SERIALIZE(TextureParallax);
public:
	TextureParallax();
	TextureParallax(char* imagePath);
	TextureParallax(char* imagePath, const Shader& shader);
	void Start()override;
	void Draw(Camera * cam)override;

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);
private:

	unsigned int t_VBO, t_VAO;
	unsigned int textureData;

	Shader m_shader;
	Color m_Color;
	Ptr(char, m_ImagePath);
	Ptr(char, m_ImageNormalPath);
	Ptr(char, m_ImageParallaxPath);
	unsigned int m_image;
	unsigned int m_imageNormal;
	unsigned int m_imageParallax;

	void renderQuad();
};

AUTO_END

#endif //!TEXTURE_PARALLAX_H_