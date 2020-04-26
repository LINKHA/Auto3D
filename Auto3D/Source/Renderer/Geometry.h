#pragma once
#include "AutoConfig.h"
#include "Container/String.h"
#include "Container/HashMap.h"
#include "Container/Pair.h"

#include <bgfx/bgfx.h>

namespace Auto3D
{
class FMeshGroup;

class AUTO_API FGeometry
{
public:
	FGeometry() 
	{
		/*_vertexBufferHandles.Resize(0);
		_indexBufferHandles.Resize(0);*/
		_name = FString::EMPTY;
		//_occlusionQuery = bgfx::createOcclusionQuery();
	}
	~FGeometry() 
	{
		/*bgfx::destroy(_occlusionQuery);
		for (auto it = _vertexBufferHandles.Begin(); it != _vertexBufferHandles.End(); ++it)
		{
			bgfx::destroy(*it);
		}
		for (auto it = _indexBufferHandles.Begin(); it != _indexBufferHandles.End(); ++it)
		{
			bgfx::destroy(*it);
		}
		_vertexBufferHandles.Clear();
		_indexBufferHandles.Clear();*/
	}
	
	FString _name;
	/// The key value of this hashmap is name, which corresponds to MeshGroup and occlusionQuery.
	TVector<TPair<FMeshGroup*, bgfx::OcclusionQueryHandle>> _geometryValue;
	//bgfx::OcclusionQueryHandle _occlusionQuery;
	//OMesh* _mesh;

	//TVector<bgfx::VertexBufferHandle> _vertexBufferHandles;
	//TVector<bgfx::IndexBufferHandle> _indexBufferHandles;

};

}