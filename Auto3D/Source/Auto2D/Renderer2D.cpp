#include "Renderer2D.h"
#include "Scene2D.h"
#include "Sprite2D.h"
#include "Camera2D.h"

#include "Graphics/Graphics.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/ConstantBuffer.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/Shader.h"
#include "Resource/ResourceCache.h"
#include "Core/Modules/ModuleManager.h"

#include "Debug/Profiler.h"
#include "Debug/DebugNew.h"

namespace Auto3D
{

/// Parameter indices in constant buffers used by high-level rendering.
static const size_t VS_FRAME_VIEW_MATRIX = 0;
static const size_t VS_FRAME_PROJECTION_MATRIX = 1;
static const size_t VS_FRAME_VIEWPROJ_MATRIX = 2;
static const size_t VS_FRAME_DEPTH_PARAMETERS = 3;
static const size_t VS_OBJECT_WORLD_MATRIX = 0;

const FString geometryDefines[] =
{
	"",
	"INSTANCED"
};

FRenderer2DModule::FRenderer2DModule() :
	_graphics(nullptr),
	_initialized(false),
	_rendered(false),
	_instanceTransformsDirty(false)
{
}

FRenderer2DModule::~FRenderer2DModule()
{
}

void FRenderer2DModule::Render(AScene2D* scene, ACamera2D* camera)
{
	PrepareView(scene, camera);

	PROFILE(Render2d);

	// Set per-frame values to the frame constant buffers
	TMatrix3x4F viewMatrix = camera->GetViewMatrix();
	TMatrix4x4F projectionMatrix = camera->GetProjectionMatrix();
	TMatrix4x4F viewProjMatrix = projectionMatrix * viewMatrix;
	TVector4F depthParameters(TVector4F::ZERO);
	depthParameters._x = camera->GetNearClip();
	depthParameters._y = camera->GetFarClip();
	if (camera->IsOrthographic())
	{
#ifdef USE_OPENGL
		depthParameters._z = 0.5f;
		depthParameters._w = 0.5f;
#else
		depthParameters._z = 1.0f;
#endif
	}
	else
		depthParameters._w = 1.0f / camera->GetFarClip();

	_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEW_MATRIX, viewMatrix);
	_vsFrameConstantBuffer->SetConstant(VS_FRAME_PROJECTION_MATRIX, projectionMatrix);
	_vsFrameConstantBuffer->SetConstant(VS_FRAME_VIEWPROJ_MATRIX, viewProjMatrix);
	
	_vsFrameConstantBuffer->SetConstant(VS_FRAME_DEPTH_PARAMETERS, depthParameters);

	_vsFrameConstantBuffer->Apply();

	_psFrameConstantBuffer->SetConstant((size_t)0, FColor::WHITE);
	_psFrameConstantBuffer->Apply();


	_graphics->SetConstantBuffer(EShaderStage::VS, EConstantBuffer2D::FRAME, _vsFrameConstantBuffer);
	_graphics->SetConstantBuffer(EShaderStage::PS, EConstantBuffer2D::FRAME, _psFrameConstantBuffer);

	_graphics->SetDepthState(ECompareFunc::LESS_EQUAL, true);
	_graphics->SetColorState(EBlendMode::ALPHA);
	_graphics->SetRasterizerState(ECullMode::BACK, EFillMode::SOLID);

	RenderBatches();
}

bool FRenderer2DModule::PrepareView(AScene2D* scend2d,ACamera2D* camera)
{
	if (!IsInitialized())
		Initialize();
	if (!Collect2dObjects(scend2d, camera))
		return false;
	

	Collect2dBatches();
	return true;
}

bool FRenderer2DModule::Collect2dObjects(AScene2D* scend2d, ACamera2D* camera)
{
	PROFILE(Collect2dObjects);

	_geometryNode.Clear();
	_batchQueue.Clear();
	_instanceTransforms.Clear();
	_scenes = scend2d;
	_camera = camera;
	//Classify Renderer2D nodes to remove nodes that are not in the view
	//\note TEMP Temporarily all join
	for (auto it = scend2d->GetAllNode().Begin(); it != scend2d->GetAllNode().End(); it++)
	{
		ANode2D* node = it->_second;
		if (node->IsEnabled() && node->TestFlag(NF_2D_GEOMETRY) && (node->GetLayerMask() & _camera->GetViewMask()))
		{
			_geometryNode.Push(static_cast<AGeometryNode2D*>(it->_second));
		}
	}

	return true;
}

void FRenderer2DModule::Collect2dBatches()
{
	PROFILE(Collect2dBatches);

	for (auto it = _geometryNode.Begin(); it != _geometryNode.End(); it++)
	{
		
		AGeometryNode2D* node = *it;

		FBatch2D newBatch;

		newBatch._type = node->GetGeometryType();
		newBatch._geometry = node->GetGeometry();
		newBatch._worldMatrix = &node->GetWorldTransform();
		newBatch._texture = node->GetTexture();

		newBatch.CalculateSortKey();
		
		_batchQueue._batches.Push(newBatch);
	}
	size_t oldSize = _instanceTransforms.Size();

	_batchQueue.Sort(_instanceTransforms);

	// Check if more instances where added
	if (_instanceTransforms.Size() != oldSize)
		_instanceTransformsDirty = true;
}

void FRenderer2DModule::RenderBatches()
{
	PROFILE(Rende2dBatches);
	RenderBatches(_batchQueue._batches, _camera);
}

void FRenderer2DModule::Initialize()
{
	auto* graphics = GModuleManager::Get().GraphicsModule();
	auto* cache = GModuleManager::Get().CacheModule();
	assert(!_graphics && !IsInitialized());

	PROFILE(InitRenderer2D);

	_graphics = graphics;
	_initialized = true;


	TVector<FConstant> constants;
	_vsFrameConstantBuffer = new FConstantBuffer();
	constants.Push(FConstant(EElementType::MATRIX3X4, "viewMatrix"));
	constants.Push(FConstant(EElementType::MATRIX4, "projectionMatrix"));
	constants.Push(FConstant(EElementType::MATRIX4, "viewProjMatrix"));
	constants.Push(FConstant(EElementType::VECTOR4, "depthParameters"));
	_vsFrameConstantBuffer->Define(EResourceUsage::DEFAULT, constants);


	_vsObjectConstantBuffer = new FConstantBuffer();
	constants.Clear();
	constants.Push(FConstant(EElementType::MATRIX3X4, "worldMatrix"));
	_vsObjectConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

	_psFrameConstantBuffer = new FConstantBuffer();
	constants.Clear();
	constants.Push(FConstant(EElementType::VECTOR4, "color"));
	_psFrameConstantBuffer->Define(EResourceUsage::DEFAULT, constants);

	// Instance vertex buffer contains texcoords 4-6 which define the instances' world matrices
	_instanceVertexBuffer = new FVertexBuffer();
	_instanceVertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TEXCOORD, U_INSTANCE_TEXCOORD, true));
	_instanceVertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TEXCOORD, U_INSTANCE_TEXCOORD + 1, true));
	_instanceVertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TEXCOORD, U_INSTANCE_TEXCOORD + 2, true));

	// Because Renderer2D images change less, their shaders are temporarily fixed
	TSharedPtr<AShader> vs(new AShader());
	TSharedPtr<AShader> ps(new AShader());
	vs = cache->LoadResource<AShader>("Shader/Texture.vert");
	ps = cache->LoadResource<AShader>("Shader/Texture.frag");
	_vsv = vs->CreateVariation();
	_psv = ps->CreateVariation();
	_ivsv = vs->CreateVariation(geometryDefines[1]);
	_ipsv = ps->CreateVariation(geometryDefines[1]);


}

void FRenderer2DModule::RenderBatches(const TVector<FBatch2D>& batches, ACamera2D* camera)
{

	if (_instanceTransformsDirty && _instanceTransforms.Size())
	{
		if (_instanceVertexBuffer->GetNumVertices() < _instanceTransforms.Size())
			_instanceVertexBuffer->Define(EResourceUsage::DYNAMIC, NextPowerOfTwo(_instanceTransforms.Size()), _instanceVertexElements, false);
		_instanceVertexBuffer->SetData(0, _instanceTransforms.Size(), &_instanceTransforms[0]);
		_graphics->SetVertexBuffer(1, _instanceVertexBuffer);
		_instanceTransformsDirty = false;
	}

	for (auto it = batches.Begin(); it != batches.End();)
	{
		const FBatch2D& batch = *it;
		bool instanced = batch._type == EGeometryType::INSTANCED;

		if (!instanced)
		{
			_vsObjectConstantBuffer->SetConstant(VS_OBJECT_WORLD_MATRIX, *batch._worldMatrix);
			_vsObjectConstantBuffer->Apply();
			_graphics->SetConstantBuffer(EShaderStage::VS, EConstantBuffer2D::OBJECT, _vsObjectConstantBuffer);
		}

		_graphics->SetTexture(0, batch._texture);
		if (instanced)
			_graphics->SetShaders(_ivsv, _ipsv);
		else
			_graphics->SetShaders(_vsv, _psv);



		FGeometry* geometry = batch._geometry;
		// Set vertex / index buffers and draw
		if (instanced)
			geometry->DrawInstanced(_graphics, batch._instanceStart, batch._instanceCount);
		else
			geometry->Draw(_graphics);

		// Advance. If instanced, skip over the batches that were converted
		it += instanced ? batch._instanceCount : 1;
	}
}

void RegisterRenderer2DLibrary()
{
	static bool registered = false;
	if (registered)
		return;
	registered = true;

	AScene2D::RegisterObject();
	ASprite2D::RegisterObject();
	ASpatialNode2D::RegisterObject();
	ACamera2D::RegisterObject();
	ANode2D::RegisterObject();
	AGeometryNode2D::RegisterObject();
}

}