#pragma once
#include "Shader.h"
#include "Transform.h"
#include "Application.h"
#include "Math/Color.h"
#include "Material.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"
#include "Model.h"


namespace Auto3D {

class Mesh : public RenderComponent,public GLMeshEnable
{
	REGISTER_DERIVED_CLASS(Mesh, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	explicit Mesh(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void SetModel(char* modelPath);
	void SetShader(const Shader& shader);

	Mesh(Ambient* ambient,char* meshPath);
	Mesh(Ambient* ambient,char* meshPath, const Shader& shader);


	void Start()override;
	void Draw()override;
	Material* GetMaterial() { return _material.get(); }

private:
	void drawMaterial();
	void drawLight();
private:
	Shader _shader;
	SharedPtr<Material> _material;
	SharedPtr<Model> _model;
	char* _modelPath;
	bool _isUserShader;
};

}

