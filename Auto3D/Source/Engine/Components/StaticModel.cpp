#pragma once

#include "Debug/Log.h"
#include "Resource/ResourceCache.h"
#include "Camera.h"
#include "Material.h"
#include "Model.h"
#include "StaticModel.h"

#include "Core/Modules/ModuleManager.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

static TVector3F DOT_SCALE(1 / 3.0f, 1 / 3.0f, 1 / 3.0f);

REGISTER_CLASS
{
	using namespace rttr;
	FRegistration::class_<AStaticModel>("StaticModel")
	.constructor<>()
		.property("modelAttr", &AStaticModel::GetModelAttr, &AStaticModel::SetModelAttr)
		(
			metadata(SERIALIZABLE, "")
		)
		.property("lodBias", &AStaticModel::GetLodBias, &AStaticModel::SetLodBias)
		(
			metadata(SERIALIZABLE, "")
		)
		;
}

AStaticModel::AStaticModel() :
    _lodBias(1.0f),
    _hasLodLevels(false)
{
}

AStaticModel::~AStaticModel()
{
}

void AStaticModel::RegisterObject()
{
    RegisterFactory<AStaticModel>();
    // Copy base attributes from AOctreeNode instead of AGeometryNode, as the model attribute needs to be set first so that
    // there is the correct amount of materials to assign
    CopyBaseAttributes<AStaticModel, AOctreeNode>();
   // RegisterMixedRefAttribute("model", &AStaticModel::GetModelAttr, &AStaticModel::SetModelAttr, FResourceRef(AModel::GetTypeStatic()));
    CopyBaseAttribute<AStaticModel, AGeometryNode>("materials");
    RegisterAttribute("lodBias", &AStaticModel::GetLodBias, &AStaticModel::SetLodBias, 1.0f);
}

void AStaticModel::OnPrepareRender(unsigned frameNumber, ACamera* camera)
{
    _lastFrameNumber = frameNumber;
    _lightList = nullptr;
    _distance = camera->Distance(GetWorldPosition());

    // Find out the new LOD level if model has LODs
    if (_hasLodLevels)
    {
        float lodDistance = camera->LodDistance(_distance, GetWorldScale().DotProduct(DOT_SCALE), _lodBias);

        for (size_t i = 0; i < _batches.Size(); ++i)
        {
            const TVector<TSharedPtr<FGeometry> >& lodGeometries = _model->GetLodGeometries(i);
            if (lodGeometries.Size() > 1)
            {
                size_t j;
                for (j = 1; j < lodGeometries.Size(); ++j)
                {
                    if (lodDistance <= lodGeometries[j]->_lodDistance)
                        break;
                }
                _batches[i]._geometry = lodGeometries[j - 1];
            }
        }
    }
}

void AStaticModel::SetModel(AModel* model)
{
    _model = model;
    _hasLodLevels = false;

    if (!_model)
    {
        SetNumGeometries(0);
        SetLocalBoundingBox(TBoundingBoxF(0.0f, 0.0f));
        return;
    }

    SetNumGeometries(_model->GetNumGeometries());
    // Start at LOD level 0
    for (size_t i = 0; i < _batches.Size(); ++i)
    {
        SetGeometry(i, _model->GetGeometry(i, 0));
        if (_model->GetNumLodLevels(i) > 1)
            _hasLodLevels = true;
    }

    SetLocalBoundingBox(_model->GetLocalBoundingBox());
}

void AStaticModel::SetLodBias(float bias)
{
    _lodBias = Max(bias, M_EPSILON);
}

AModel* AStaticModel::GetModel() const
{
    return _model.Get();
}

void AStaticModel::SetModelAttr(FResourceRef model)
{
	FResourceModule* cache = GModuleManager::Get().CacheModule();
    SetModel(cache->LoadResource<AModel>(model._name));
}

FResourceRef AStaticModel::GetModelAttr() const
{
    return FResourceRef(AModel::GetTypeStatic(), ResourceName(_model.Get()));
}

}
