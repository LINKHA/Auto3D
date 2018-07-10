#ifndef TEXTURE_MAP_H_
#define TEXTURE_MAP_H_
#include "Texture.h"
#include "Texture.h"
#include "GLStateCache.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
AUTO_BEGIN

class TextureMap : public Texture
{
public:
	typedef struct _TexParams {
		GLuint    minFilter;
		GLuint    magFilter;
		GLuint    wrapS;
		GLuint    wrapT;
	}TexParams;

public:

	TextureMap();
	TextureMap(char* imagePath);
	TextureMap(char* imagePath, const Shader& shader);
	~TextureMap();
	void Start()override;
	void Draw(Camera * cam)override;


	void SetTexParameters(const TexParams& params);
	void GenerateMipmap();

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	bool useStencil;
	bool useDepth;
	bool useBlend;

	bool useParallax;
	void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void StencilFunc(GLenum func, GLint ref, GLuint mask);
	void StencilMask(GLuint mask);
	void DepthFunc(GLenum func);

	
protected:

	float width;
	float height;
	bool is_Mipmaps;

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
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;

	
	void renderQuad();
};

AUTO_END

#endif //!TEXTURE_MAP_H_