#pragma once
#include "Shader.h"
#include "Transform.h"
#include "Application.h"
#include "Color.h"
#include "Material.h"
#include "RenderComponent.h"
#include "GLMeshEnable.h"
#include "Mesh.h"


namespace Auto3D {

class MeshRenderer : public RenderComponent, public GLMeshEnable, public EnableSharedFromThis<MeshRenderer>
{
	REGISTER_OBJECT_CLASS(MeshRenderer, RenderComponent)
public:
	explicit MeshRenderer(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);

	void Destory()override;

	void Start()override;
	void Draw()override;

	void SetMesh(SharedPtr<Mesh> mesh);

	void SetShaderVariation(SharedPtr<ShaderVariation> shader);

	SharedPtr<Material> GetMaterial() { return _material; }
private:
	void drawMaterial();
	void drawLight();
private:
	SharedPtr<Material> _material;
	SharedPtr<Mesh> _mesh;
	bool _isUserShader;

	SharedPtr<ShaderVariation> _shader;
};

}

