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

class MeshRenderer : public RenderComponent, public GLMeshEnable
{
	REGISTER_OBJECT_CLASS(MeshRenderer, RenderComponent)
public:
	/**
	* @brief : Construct
	*/
	explicit MeshRenderer(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);
	/**
	* @brief : Override Start
	*/
	void Start()override;
	/**
	* @brief : Override Draw
	*/
	void Draw()override;
	/**
	* @brief : Destory mesh renderer to unload opaque
	*/
	void Destory()override;
	/** 
	* @brief : Set mesh to mesh renderer
	*/
	void SetMesh(SharedPtr<Mesh> mesh);
	/**
	* @brief : Set shader variation
	*/
	void SetShaderVariation(SharedPtr<ShaderVariation> shader);
	/**
	* @brief : Get material
	*/
	SharedPtr<Material> GetMaterial() { return _material; }
private:
	/** 
	* @brief : Draw material 
	*/
	void drawMaterial();
	/**
	* @brief : Draw light
	*/
	void drawLight();
private:
	/// Material
	SharedPtr<Material> _material;
	/// MeshRenderer mesh
	SharedPtr<Mesh> _mesh;
	/// Is user shader
	bool _isUserShader;
	/// Shader
	SharedPtr<ShaderVariation> _shader;
};

}

