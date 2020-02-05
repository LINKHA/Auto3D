#pragma once
#include "AutoConfig.h"
namespace Auto3D
{

struct AUTO_API FBatch
{
	/*/// Calculate sort _key for state sorting.
	void CalculateSortKey()
	{
		_sortKey = ((((unsigned long long)_pass->GetShaderHash() * _type) & 0xffff) << 48) |
			((((unsigned long long)_lights) & 0xffff) << 32) |
			((((unsigned long long)_pass->Parent()) & 0xffff) << 16) |
			(((unsigned long long)_geometry) & 0xffff);
	}
	/// FGeometry.
	FGeometry* _geometry;

	Shader
	/// Sort _key for state sorting.
	unsigned long long _sortKey;*/
};

}