#pragma once
#include "AutoConfig.h"
#include "Renderer/ShaderProgram.h"
#include "Container/String.h"
#include "Container/StringHash.h"

namespace Auto3D
{

struct AUTO_API FPass
{
	FPass():
		_shaderHash(0),
		_geometryHash(0)
	{}

	void CreateShaderHash(FString vsName = FString::EMPTY,FString psName = FString::EMPTY)
	{
		_shaderHash = FStringHash(vsName + psName).Value();
	}

	void CreateGeometryHash(const FString& geometryName)
	{
		_geometryHash = FStringHash(geometryName).Value();
	}

	bool operator==(const FPass& rhs)
	{
		if (_shaderHash == rhs._shaderHash
			&& _geometryHash == rhs._geometryHash)
			return true;

		return false;
	}

	unsigned _shaderHash;
	unsigned _geometryHash;
};

}