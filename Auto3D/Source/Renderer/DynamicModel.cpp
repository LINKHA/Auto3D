#pragma once

#include "../Debug/Log.h"
#include "../Resource/ResourceCache.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "DynamicModel.h"

#include "../Engine/ModuleManager.h"
#include "../Debug/DebugNew.h"

namespace Auto3D
{

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


}
