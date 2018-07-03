#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_
#include "Texture.h"
#include "GLStateCache.h"
#include "LoadResource.h"
#include "Transform.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
USING_MATH
AUTO_BEGIN


class Texture2D : public Texture
{
	REGISTER_DERIVED_CLASS(Texture2D, Texture);
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
	Texture2D(char* imagePath);
	Texture2D(char* imagePath, const Shader& shader);

	void Start();
	void Draw(Camera * cam);

	void SetLinerParameters();
	void SetNearestParameters();
	void SetTexParameters(const TexParams& params);
	void GenerateMipmap();

	void SetColor(const Color& color);
	void SetColor(const Vector3& vec);
	void SetColor(float r, float g, float b, float a = 1.0f);

	bool useStencil;
	bool useDepth;
	bool useBlend;
	void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void StencilFunc(GLenum func, GLint ref, GLuint mask);
	void StencilMask(GLuint mask);
	void DepthFunc(GLenum func);
protected:

	float width;
	float height;
	bool is_Mipmaps;

private:

	unsigned int t_VBO, t_VAO, t_EBO;
	unsigned int textureData;
	Shader m_shader;
	Color m_Color;
	Ptr(char,m_ImagePath);
	Ptr(Image, m_image);
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
};

AUTO_END
#endif //!TEXTURE_2D_H_