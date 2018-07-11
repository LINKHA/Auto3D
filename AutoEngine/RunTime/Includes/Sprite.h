#ifndef SPRITE_H_
#define SPRITE_H_
#include "Texture2D.h"
AUTO_BEGIN
class Sprite : public Texture2D
{
	REGISTER_DERIVED_CLASS(Sprite, Texture2D);
	DECLARE_OBJECT_SERIALIZE(Sprite);
public:

	Sprite();
	Sprite(char* imagePath);
	Sprite(char* imagePath, const Shader& shader);

	void Start()override;
	void Draw(Camera * cam)override;

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	void SetLinerParameters();
	void SetNearestParameters();

	void SetTexParameters(const TexParams& params);
	void GenerateMipmap();
private:
	unsigned int t_VBO, t_VAO, t_EBO;
	unsigned int textureData;

	Shader m_shader;
	Color m_Color;
	Ptr(char, m_ImagePath);
	Ptr(Image, m_image);


	bool is_Mipmaps;
};
AUTO_END
#endif