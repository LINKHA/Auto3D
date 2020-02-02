#pragma once
#include "Component/SceneComponent.h"
#include "Resource/Mesh.h"
namespace Auto3D
{

class AUTO_API AMeshComponent : public ASceneComponent
{
	DECLARE_A_CLASS(AMeshComponent, ASceneComponent)
public:
	AMeshComponent() {}
	~AMeshComponent() {}
	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

	/// Set the mesh resource.
	void SetMesh(SPtr<OMesh>& mesh);
	/// Return the mesh resource.
	SPtr<OMesh>& GetMesh() { return _mesh; }
public:
	/// Set mesh attribute. Used in serialization.
	void SetMeshAttr(FResourceRef model);
	/// Return mesh attribute. Used in serialization.
	FResourceRef GetMeshAttr() const;
private:
	/// Current mesh resource.
	SPtr<OMesh> _mesh;
};
}