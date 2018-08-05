//#pragma once
//#include "Shader.h"
//#include "GLMeshEnable.h"
//#include "RenderComponent.h"
//
//namespace Auto3D {
//
//class ModelCommand;
//class Camera;
//class MeshShadow : public RenderComponent,public GLMeshEnable
//{
//public:
//	explicit MeshShadow(Ambient* ambient);
//	//MeshShadow(int i);
//	//int k;
//	~MeshShadow();
//	void Start()override;
//	void Draw()override;
//	void DrawShadow()override;
//
//	/*void Draw(const Shader &shader);
//	void Draw2(Camera* camera, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);*/
//
//private:
//	Shader _shader;
//	ModelCommand* _model;
//	unsigned int _woodTexture;
//	unsigned int _VAO;
//	unsigned int _VBO;
//
//};
//
//}


#pragma once
#include "Shader.h"
#include "ModelCommand.h"
#include "Transform.h"
#include "Application.h"
#include "LoadResource.h"
#include "Math/Color.h"
#include "Material.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"

namespace Auto3D {

	class MeshShadow : public GLMeshEnable
	{

	public:
		MeshShadow();
		MeshShadow(int i);
		int k;
		~MeshShadow();
		void Start();
		void Draw(const Shader &shader);
		void Draw2(Camera* camera, glm::vec3 lightPos, glm::mat4 lightSpaceMatrix);

	private:
		Shader _shader;
		ModelCommand* _model;
		unsigned int _woodTexture;
		unsigned int _VAO;
		unsigned int _VBO;

	};

}

