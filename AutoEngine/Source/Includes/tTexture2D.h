#pragma once
#include "tTexture.h"

namespace Auto3D {
class tTexture2D : public tTexture
{
	REGISTER_OBJECT_ABSTRACT_CLASS(tTexture2D, tTexture)
public:
	typedef struct _TexParams {
		GLuint    minFilter;
		GLuint    magFilter;
		GLuint    wrapS;
		GLuint    wrapT;
	}TexParams;

public:
	explicit tTexture2D(SharedPtr<Ambient> ambient);
	
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
	virtual void Draw() {}

};

}
