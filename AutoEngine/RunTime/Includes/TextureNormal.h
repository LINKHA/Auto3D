#pragma once
#include "Texture.h"
#include "Texture.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
#include "TextureMap.h"
AUTO_BEGIN

class TextureNormal : public TextureMap
{
	REGISTER_DERIVED_CLASS(TextureNormal, TextureMap);
	DECLARE_OBJECT_SERIALIZE(TextureNormal);
public:
	TextureNormal();
	TextureNormal(char* imagePath);
	TextureNormal(char* imagePath, const Shader& shader);
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
	unsigned int m_image;
	unsigned int m_imageNormal;

	void renderQuad();
};

AUTO_END
