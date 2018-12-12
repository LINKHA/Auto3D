#pragma once
#include "_Shader.h"
#include "Transform.h"
#include "Application.h"
#include "Math/Color.h"
#include "Material.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"
#include "Mesh.h"


namespace Auto3D {

class MeshRenderer : public RenderComponent, public GLMeshEnable
{
	REGISTER_OBJECT_CLASS(MeshRenderer, RenderComponent)
public:
	explicit MeshRenderer(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;
	void Draw()override;

	void SetMesh(Mesh* mesh);

	void SetShader(const _Shader& shader);

	Material* GetMaterial() { return _material.get(); }
private:
	void drawMaterial();
	void drawLight();
private:
	_Shader _shader;
	SharedPtr<Material> _material;
	SharedPtr<Mesh> _mesh;
	bool _isUserShader;
};

}

