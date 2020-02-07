#pragma once
#include "Component/ActorComponent.h"
#include "Resource/ResourceRef.h"
#include "Renderer/Pass.h"

namespace Auto3D
{
class OMaterial;
class FPass;
class FGeometry;

/// Draw call source data.
struct AUTO_API FSourceBatch
{
	/// The geometry to render. Must be non-null.
	FGeometry* _geometry;
	/// The material to use for rendering. Must be non-null.
	OMaterial* _material;
};

class AUTO_API AGeometryComponent : public AActorComponent
{
	DECLARE_A_CLASS(AGeometryComponent, AActorComponent)
public:
	AGeometryComponent() {}
	virtual ~AGeometryComponent() {}

	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	void SetMaterial(OMaterial* material);
	OMaterial* GetMaterial() const;

	void SetGeometryName(const FString& name);
	const FPass& GetPass();

	/// Set geometry at index.
	void _SetGeometry(size_t index, FGeometry* geometry);
	/// Set material at every geometry index. Specifying null will use the default material (opaque white.)
	void _SetMaterial(OMaterial* material);
	/// Set material at geometry index.
	void _SetMaterial(size_t index, OMaterial* material);


	/// Return source information for all draw calls.
	const TVector<FSourceBatch>& GetBatches() const { return _batches; }
public:
	/// Handle being assigned to a new parent node.
	virtual void OnActorSet(AActor* newParent, AActor* oldParent);
private:
	FPass _pass;

	OMaterial* _material;

	FGeometry* _geometrys;

	/// Draw call source datas.
	TVector<FSourceBatch> _batches;
};

}