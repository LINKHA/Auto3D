#pragma once
#include "Component/SceneComponent.h"
#include "Resource/Mesh.h"
namespace Auto3D
{

class AUTO_API AMeshComponent : public ASceneComponent
{
	DECLARE_CLASS(AMeshComponent, ASceneComponent)
public:
	AMeshComponent() {}
	~AMeshComponent() {}
	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

	/// Set the mesh resource.
	void SetMesh(SPtr<FMesh>& mesh);

	SPtr<FMesh>& GetMesh() { return _mesh; }
private:
	/// Current mesh resource.
	SPtr<FMesh> _mesh;
};
}