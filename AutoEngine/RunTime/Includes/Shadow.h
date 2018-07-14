#ifndef SHADOW_H_
#define SHADOW_H_
#include "Auto.h"
#include "Shader.h"
#include "GameObject.h"
#include "MeshShadow.h"
AUTO_BEGIN

class Shadow : public Component
{
public:
	Shadow();
	virtual ~Shadow();
	void Start()override;
	void Draw(Camera* camera = nullptr)override;
private:

	Shader m_ShadowMapDepth;
	glm::vec3 lightPos;
	unsigned int woodTexture;
	unsigned int planeVAO;

	MeshShadow* mesh;
	MeshShadow* mesh1;
	MeshShadow* mesh2;
	MeshShadow* mesh3;
};

AUTO_END
#endif //!SHADOW_H_
