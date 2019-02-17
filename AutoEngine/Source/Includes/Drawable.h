#pragma once
#include "GraphicsDef.h"

namespace Auto3D {

class Geometry;

/// Source data for a 3D geometry draw call.
struct SourceBatch
{
	/// Construct with defaults.
	SourceBatch();
	/// Copy-construct.
	SourceBatch(const SourceBatch& batch);
	/// Destruct.
	~SourceBatch();

	/// Assignment operator.
	SourceBatch& operator =(const SourceBatch& rhs);

	/// Distance from camera.
	float distance_{};
	/// Geometry.
	SharedPtr<Geometry> geometry_{};

	/// World transform(s). For a skinned model, these are the bone transforms.
	//const Matrix3x4* worldTransform_{ &Matrix3x4::IDENTITY };
	/// Number of world transforms.
	unsigned numWorldTransforms_{ 1 };
	/// Per-instance data. If not null, must contain enough data to fill instancing buffer.
	void* instancingData_{};
	/// %Geometry type.
	GeometryType geometryType_{ GeometryType::Static };
};


}