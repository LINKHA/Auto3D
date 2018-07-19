#pragma once
#include "Shader.h"
#include "ModelCommand.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Math/Color.h"
#include "AuMaterial.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"

AUTO_BEGIN

class MeshShadow : public GLMeshEnable
{

public:
	MeshShadow();
	MeshShadow(int i);
	int k;
	~MeshShadow();
	void Start();
	void Draw(const Shader &shader);
	void Draw2(Camera * camera, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);

private:
	Shader m_shader;
	unsigned int m_woodTexture;
	unsigned int m_VAO ;
	unsigned int m_VBO;
	
};

AUTO_END

