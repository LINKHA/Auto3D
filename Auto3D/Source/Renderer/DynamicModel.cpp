#pragma once

#include "../Debug/Log.h"
#include "../Resource/ResourceCache.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "DynamicModel.h"

#include "../Engine/ModuleManager.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

static const int WATER_PLANE_LENGTH = 128;

static Vector3F DOT_SCALE(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);

DynamicModel::DynamicModel()
{
}

DynamicModel::~DynamicModel()
{
}

void DynamicModel::RegisterObject()
{
	RegisterFactory<DynamicModel>();
}

void DynamicModel::OnPrepareRender(unsigned frameNumber, Camera* camera)
{
	_lastFrameNumber = frameNumber;
	_lightList = nullptr;
	_distance = camera->Distance(GetWorldPosition());
}

void DynamicModel::SetWaterData()
{
	static float* waterPoints = (float*)malloc(WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(float));
	static unsigned int* waterIndices = (unsigned int*)malloc(WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(unsigned int));
	static bool waterPointsDirty = false;
	static bool waterIndeicesDirty = false;

	if (!waterPointsDirty)
	{
		waterPointsDirty = true;
		for (int i = 0; i < WATER_PLANE_LENGTH; i++)
		{
			for (int k = 0; k < WATER_PLANE_LENGTH; k++)
			{
				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 0] = -(float)WATER_PLANE_LENGTH / 2 + 0.5f + (float)k;
				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 1] = 0.0f;
				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 2] = +(float)WATER_PLANE_LENGTH / 2 - 0.5f - (float)i;
				waterPoints[(k + i * (WATER_PLANE_LENGTH)) * 4 + 3] = 1.0f;
			}
		}
	}
	
	if (!waterIndeicesDirty)
	{
		waterIndeicesDirty = true;
		for (int i = 0; i < WATER_PLANE_LENGTH - 1; i++)
		{
			for (int k = 0; k < WATER_PLANE_LENGTH; k++)
			{
				if (i % 2 == 0)
				{
					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 0] = k + (i + 1) * WATER_PLANE_LENGTH;
					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 1] = k + i * WATER_PLANE_LENGTH;
				}
				else
				{
					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 0] = WATER_PLANE_LENGTH - 1 - k + i * WATER_PLANE_LENGTH;
					waterIndices[(k + i * (WATER_PLANE_LENGTH)) * 2 + 1] = WATER_PLANE_LENGTH - 1 - k + (i + 1) * WATER_PLANE_LENGTH;
				}
			}
		}
	}



	SetNumGeometries(1);

	Vector<VertexElement> vertexDeclaration;
	vertexDeclaration.Push(VertexElement(ElementType::VECTOR4, ElementSemantic::POSITION));//vertex

	SharedPtr<VertexBuffer> vertexBuffer;
	vertexBuffer->Define(ResourceUsage::IMMUTABLE, 4, vertexDeclaration, true, waterPoints);


	SharedPtr<IndexBuffer> indexBuffer;
	indexBuffer->Define(ResourceUsage::IMMUTABLE, WATER_PLANE_LENGTH * WATER_PLANE_LENGTH, sizeof(unsigned short), true, waterIndices);


	SharedPtr<Geometry> geometry(new Geometry());
	// Temp use this setting
	geometry->_lodDistance = 0.0f;
	geometry->_primitiveType = PrimitiveType::TRIANGLE_LIST;
	geometry->_drawStart = 0;
	geometry->_drawCount = 0;

	geometry->_vertexBuffer = vertexBuffer;
	geometry->_indexBuffer = indexBuffer;

	SetGeometry(0, geometry);

}

}
