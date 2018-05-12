#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_
#include "BaseTexture.h"
#include "GLStateCache.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
USING_MATH
AUTO_BEGIN
class Texture2D : public BaseTexture
{
	REGISTER_DERIVED_CLASS(Texture2D, BaseTexture);
	DECLARE_OBJECT_SERIALIZE(Texture2D);
public:
	typedef struct _TexParams {
		GLuint    minFilter;
		GLuint    magFilter;
		GLuint    wrapS;
		GLuint    wrapT;
	}TexParams;

public:

	Texture2D();
	Texture2D(_String imagePath, const Transform& transform = Transform());
	Texture2D(_String imagePath, const Shader& shader, const Transform& transform = Transform());

	void draw();
	//void draw(Rect rec);

	void pushToRunloop();

	void setLinerParameters();

	void setNearestParameters();

	void setTexParameters(const TexParams& params);

	void generateMipmap();

	void setColor(const Color& color);
protected:

	float width;

	float height;

	bool is_Mipmaps;

private:

	unsigned int t_VBO, t_VAO, t_EBO;

	unsigned int textureData;

	Transform m_transform;

	Shader m_shader;

	_String m_ImagePath;

	Color m_color;
};

AUTO_END
#endif //!TEXTURE_2D_H_