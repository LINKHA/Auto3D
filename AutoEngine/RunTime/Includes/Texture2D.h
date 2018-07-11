#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_
#include "Texture.h"
#include "GLStateCache.h"
#include "LoadResource.h"
#include "Camera.h"
#include "Application.h"
#include "GameObject.h"
USING_MATH
AUTO_BEGIN


class Texture2D : public Texture
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Texture2D, Texture);
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
	
	void Start()override;
	void Draw(Camera * cam)override;


	bool useStencil;
	bool useDepth;
	bool useBlend;
	void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	void StencilFunc(GLenum func, GLint ref, GLuint mask);
	void StencilMask(GLuint mask);
	void DepthFunc(GLenum func);
protected:
	GLenum m_sfail; GLenum m_dpfail; GLenum m_dppass;
	GLenum m_func; GLint m_ref; GLuint m_mask;
	GLuint m_mas;
	GLenum m_depthfunc;
};

AUTO_END
#endif //!TEXTURE_2D_H_