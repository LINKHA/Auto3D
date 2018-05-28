#ifndef RENDERER_H_ 
#define RENDERER_H_
#include "Singleton.h"

AUTO_BEGIN
class Renderer : public Singleton<Renderer>
{
public:
	Renderer();
	~Renderer();
	void Render();
	void Clean();
	void Clear();
	void RenderLoop();
};
AUTO_END

#endif // !BASE_RENDERER_H_
