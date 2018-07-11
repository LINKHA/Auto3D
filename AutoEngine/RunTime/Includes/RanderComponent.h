#ifndef RENDER_COMPONENT_H_
#define RENDER_COMPONENT_H_
#include "GameObject.h"
#include "OpenGLGather.h"

AUTO_BEGIN

class RanderComponent : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(RanderComponent, Component);
	DECLARE_OBJECT_SERIALIZE(RanderComponent);
public:
	RanderComponent();
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw(Camera* camera = nullptr) {}

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

#endif //!RENDER_COMPONENT_H_