#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Container/String.h"
#include "Container/StringHash.h"
#include "Renderer/Geometry.h"
#include "Resource/Material.h"
#include "Resource/Shader.h"
#include "Math/Matrix3x4.h"

namespace Auto3D
{

struct AUTO_API FGeometryPass
{
	FGeometryPass():
		_sortKey(0),
		_material(nullptr),
		_geometry(nullptr),
		_worldMatrix(nullptr)
	{}

	bool operator==(const FGeometryPass& rhs)
	{
		if (_sortKey == rhs._sortKey)
			return true;

		return false;
	}
	
	void CalculateSortKey()
	{
		if (!_material || !_geometry)
			return;

		unsigned shaderHash;

		if (_material->GetShaderType() == EMaterialShaderType::DEFAULT)
		{
			shaderHash = FStringHash(_material->GetShaderProgram().GetVertexShader()->GetPathName()
				+ _material->GetShaderProgram().GetPixelShader()->GetPathName()).Value();
		}
		else
		{
			shaderHash = FStringHash(_material->GetShaderType()).Value();
		}


		_sortKey = ((((unsigned long long)shaderHash) & 0xffffffff) << 32)|
		((unsigned long long)FStringHash(_geometry->_name).Value() & 0xffffffff);
	}

	bool isValid()
	{
		return _geometry && _material && _worldMatrix;
	}

	/// Draw call source datas.
	OMaterial* _material;

	/// Draw call source datas.
	FGeometry* _geometry;

	/// Non-instanced use world matrix.
	const TMatrix3x4F* _worldMatrix;

	/// Sort _key for state sorting.
	unsigned long long _sortKey;
};

}