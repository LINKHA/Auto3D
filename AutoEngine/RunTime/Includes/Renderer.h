#ifndef RENDERER_H_ 
#define RENDERER_H_

class Renderer
{
public:
	Renderer();
	~Renderer();


	void Render();

	void Clean();

	void Clear();

	void RenderLoop();
};
#endif // !BASE_RENDERER_H_
