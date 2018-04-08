#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_
#include "BaseTexture.h"
#include "GLStateCache.h"
USING_MATH
AUTO_BEGIN
class Texture2D : public BaseTexture
{
public:
	Texture2D();
	Texture2D(const Shader& shader);
	virtual ~Texture2D();


	void draw(const Vector2& vec);
	void draw(const Vector3& vec);
	//void draw(Rect rec);

	void pushToRunloop();

	void updateData(const Vector3& position);
	void updateData(const Vector2& position);
//	void updateData(const Texture& texture);


protected:
	int _pixelsWide;

	/** height in pixels */
	int _pixelsHigh;

	/** texture name */
	GLuint _name;

	/** texture max S */
	GLfloat _maxS;

	/** texture max T */
	GLfloat _maxT;

	/** whether or not the texture has their Alpha premultiplied */
	bool _hasPremultipliedAlpha;

	/** whether or not the texture has mip maps*/
	bool _hasMipmaps;

private:
	Shader m_shader;
	unsigned int VBO, VAO, EBO;
	unsigned int texture;
};

AUTO_END
#endif //!TEXTURE_2D_H_