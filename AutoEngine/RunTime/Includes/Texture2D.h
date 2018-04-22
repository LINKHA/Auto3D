#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_
#include "BaseTexture.h"
#include "GLStateCache.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
USING_MATH
AUTO_BEGIN
class Texture2D : public BaseTexture
{
public:
	typedef struct _TexParams {
		GLuint    minFilter;
		GLuint    magFilter;
		GLuint    wrapS;
		GLuint    wrapT;
	}TexParams;

public:

	Texture2D();
	Texture2D(const Shader& shader);
	virtual ~Texture2D();


	void draw(const Vector2& vec);
	void draw(const Vector3& vec);
	//void draw(Rect rec);

	void pushToRunloop();
	
	void setLinerParameters();

	void setNearestParameters();

	void setTexParameters(const TexParams& params);

	void generateMipmap();

protected:

	float width;		

	float height;

	bool is_Mipmaps;

private:
	Shader m_shader;
	
	unsigned int t_VBO, t_VAO, t_EBO;

	unsigned int textureData; 

	Transform m_transform;

	//Camera m_camera;
};

AUTO_END
#endif //!TEXTURE_2D_H_