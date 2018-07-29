#pragma once
#include "Shader.h"
#include "ModelCommand.h"
#include "Transform.h"
#include "Application.h"
#include "AtConfig.h"
#include "LoadResource.h"
#include "Math/Color.h"
#include "Material.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"

AUTO_BEGIN

class Mesh : public RenderComponent,public GLMeshEnable
{
	REGISTER_DERIVED_CLASS(Mesh, RenderComponent);
	DECLARE_OBJECT_SERIALIZE(Mesh);
public:
	explicit Mesh(Ambient* ambient);
	Mesh(Ambient* ambient,char* meshPath);
	Mesh(Ambient* ambient,char* meshPath, const Shader& shader);

	void Start()override;
	void Draw()override;
	Material& GetMaterial() { return _material; }

private:
	void drawMaterial();
	void drawLight();
private:
	Shader _shader;
	Material _material;
	ModelCommand* _model;
	Ptr(char, _meshPath);
private:
	bool _isUserShader;
};

AUTO_END

