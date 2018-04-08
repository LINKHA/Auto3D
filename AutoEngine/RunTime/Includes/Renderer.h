#ifndef RENDERER_H_ 
#define RENDERER_H_

class Renderer
{
public:
	Renderer();
	~Renderer();


	void render();

	void clean();

	void clear();

	void renderLoop();
};
#endif // !BASE_RENDERER_H_
