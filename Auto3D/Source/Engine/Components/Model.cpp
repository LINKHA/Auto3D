#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Scene/Node.h"
#include "Scene/GeometryNode.h"
#include "Material.h"
#include "Model.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

FBone::FBone() :
    _initialPosition(Vector3F::ZERO),
    _initialRotation(Quaternion::IDENTITY),
    _initialScale(Vector3F::ONE),
    _offsetMatrix(Matrix3x4F::IDENTITY),
    _radius(0.0f),
    _boundingBox(0.0f, 0.0f),
    _parentIndex(0),
    _animated(true)
{
}

FBone::~FBone()
{
}

AModel::AModel()
{
}

AModel::~AModel()
{
}

void AModel::RegisterObject()
{
    RegisterFactory<AModel>();
}

bool AModel::BeginLoad(FStream& source)
{
    /// \todo Develop own format for Auto3D
    if (source.ReadFileID() != "UMDL")
    {
        ErrorString(source.FName() + " is not a valid model file");
        return false;
    }

    _vbDescs.Clear();
    _ibDescs.Clear();
    _geomDescs.Clear();

    size_t numVertexBuffers = source.Read<unsigned>();
    _vbDescs.Resize(numVertexBuffers);
    for (size_t i = 0; i < numVertexBuffers; ++i)
    {
        VertexBufferDesc& vbDesc = _vbDescs[i];

        vbDesc._numVertices = source.Read<unsigned>();
        unsigned elementMask = source.Read<unsigned>();
        source.Read<unsigned>(); // morphRangeStart
        source.Read<unsigned>(); // morphRangeCount

        size_t vertexSize = 0;
        if (elementMask & 1)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR3, EElementSemantic::POSITION));
            vertexSize += sizeof(Vector3F);
        }
        if (elementMask & 2)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR3, EElementSemantic::NORMAL));
            vertexSize += sizeof(Vector3F);
        }
        if (elementMask & 4)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::UBYTE4, EElementSemantic::COLOR));
            vertexSize += 4;
        }
        if (elementMask & 8)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR2, EElementSemantic::TEXCOORD));
            vertexSize += sizeof(Vector2F);
        }
        if (elementMask & 16)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR2, EElementSemantic::TEXCOORD, 1));
            vertexSize += sizeof(Vector2F);
        }
        if (elementMask & 32)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR3, EElementSemantic::TEXCOORD));
            vertexSize += sizeof(Vector3F);
        }
        if (elementMask & 64)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR3, EElementSemantic::TEXCOORD, 1));
            vertexSize += sizeof(Vector3F);
        }
        if (elementMask & 128)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::TANGENT));
            vertexSize += sizeof(Vector4F);
        }
        if (elementMask & 256)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::VECTOR4, EElementSemantic::BLENDWEIGHT));
            vertexSize += sizeof(Vector4F);
        }
        if (elementMask & 512)
        {
            vbDesc._vertexElements.Push(FVertexElement(EElementType::UBYTE4, EElementSemantic::BLENDINDICES));
            vertexSize += 4;
        }

        vbDesc._vertexData = new unsigned char[vbDesc._numVertices * vertexSize];
        source.Read(&vbDesc._vertexData[0], vbDesc._numVertices * vertexSize);
    }

    size_t numIndexBuffers = source.Read<unsigned>();
    _ibDescs.Resize(numIndexBuffers);
    for (size_t i = 0; i < numIndexBuffers; ++i)
    {
        IndexBufferDesc& ibDesc = _ibDescs[i];
    
        ibDesc._numIndices = source.Read<unsigned>();
        ibDesc._indexSize = source.Read<unsigned>();
        ibDesc._indexData = new unsigned char[ibDesc._numIndices * ibDesc._indexSize];
        source.Read(&ibDesc._indexData[0], ibDesc._numIndices * ibDesc._indexSize);
    }

    size_t numGeometries = source.Read<unsigned>();

    _geomDescs.Resize(numGeometries);
    _boneMappings.Resize(numGeometries);
    for (size_t i = 0; i < numGeometries; ++i)
    {
        // Read bone mappings
        size_t boneMappingCount = source.Read<unsigned>();
        _boneMappings[i].Resize(boneMappingCount);
        /// \todo Should read as a batch
        for (size_t j = 0; j < boneMappingCount; ++j)
            _boneMappings[i][j] = source.Read<unsigned>();

        size_t numLodLevels = source.Read<unsigned>();
        _geomDescs[i].Resize(numLodLevels);

        for (size_t j = 0; j < numLodLevels; ++j)
        {
            GeometryDesc& geomDesc = _geomDescs[i][j];

            geomDesc._lodDistance = source.Read<float>();
            source.Read<unsigned>(); // Primitive type
            geomDesc._primitiveType = EPrimitiveType::TRIANGLE_LIST; // Always assume triangle list for now
            geomDesc._vbRef = source.Read<unsigned>();
            geomDesc._ibRef = source.Read<unsigned>();
            geomDesc._drawStart = source.Read<unsigned>();
            geomDesc._drawCount = source.Read<unsigned>();
        }
    }

    // Read (skip) morphs
    size_t numMorphs = source.Read<unsigned>();
    if (numMorphs)
    {
        ErrorString("Models with vertex morphs are not supported for now");
        return false;
    }

    // Read skeleton
    size_t numBones = source.Read<unsigned>();
    _bones.Resize(numBones);
    for (size_t i = 0; i < numBones; ++i)
    {
        FBone& bone = _bones[i];
        bone._name = source.Read<FString>();
        bone._parentIndex = source.Read<unsigned>();
        bone._initialPosition = source.Read<Vector3F>();
        bone._initialRotation = source.Read<Quaternion>();
        bone._initialScale = source.Read<Vector3F>();
        bone._offsetMatrix = source.Read<Matrix3x4F>();

        unsigned char boneCollisionType = source.Read<unsigned char>();
        if (boneCollisionType & 1)
            bone._radius = source.Read<float>();
        if (boneCollisionType & 2)
            bone._boundingBox = source.Read<BoundingBoxF>();

        if (bone._parentIndex == i)
            _rootBoneIndex = i;
    }

    // Read bounding box
    _boundingBox = source.Read<BoundingBoxF>();

    return true;
}

bool AModel::EndLoad()
{
    TVector<TSharedPtr<FVertexBuffer> > vbs;
    for (size_t i = 0; i < _vbDescs.Size(); ++i)
    {
        const VertexBufferDesc& vbDesc = _vbDescs[i];
        TSharedPtr<FVertexBuffer> vb(new FVertexBuffer());

        vb->Define(EResourceUsage::IMMUTABLE, vbDesc._numVertices, vbDesc._vertexElements, true, vbDesc._vertexData.Get());
        vbs.Push(vb);
    }

    TVector<TSharedPtr<FIndexBuffer> > ibs;
    for (size_t i = 0; i < _ibDescs.Size(); ++i)
    {
        const IndexBufferDesc& ibDesc = _ibDescs[i];
        TSharedPtr<FIndexBuffer> ib(new FIndexBuffer());

        ib->Define(EResourceUsage::IMMUTABLE, ibDesc._numIndices, ibDesc._indexSize, true, ibDesc._indexData.Get());
        ibs.Push(ib);
    }

    // Set the buffers to each geometry
    _geometries.Resize(_geomDescs.Size());
    for (size_t i = 0; i < _geomDescs.Size(); ++i)
    {
        _geometries[i].Resize(_geomDescs[i].Size());
        for (size_t j = 0; j < _geomDescs[i].Size(); ++j)
        {
            const GeometryDesc& geomDesc = _geomDescs[i][j];
            TSharedPtr<Geometry> geom(new Geometry());

            geom->_lodDistance = geomDesc._lodDistance;
            geom->_primitiveType = geomDesc._primitiveType;
            geom->_drawStart = geomDesc._drawStart;
            geom->_drawCount = geomDesc._drawCount;
            
            if (geomDesc._vbRef < vbs.Size())
                geom->_vertexBuffer = vbs[geomDesc._vbRef];
            else
                ErrorString("Out of range vertex buffer reference in " + FName());

            if (geomDesc._ibRef < ibs.Size())
                geom->_indexBuffer = ibs[geomDesc._ibRef];
            else
                ErrorString("Out of range index buffer reference in " + FName());
            
            _geometries[i][j] = geom;
        }
    }

    _vbDescs.Clear();
    _ibDescs.Clear();
    _geomDescs.Clear();

    return true;
}

void AModel::SetNumGeometries(size_t num)
{
    _geometries.Resize(num);
    // Ensure that each geometry has at least 1 LOD level
    for (size_t i = 0; i < _geometries.Size(); ++i)
    {
        if (!_geometries[i].Size())
            SetNumLodLevels(i, 1);
    }
}

void AModel::SetNumLodLevels(size_t index, size_t num)
{
    if (index >= _geometries.Size())
    {
        ErrorString("Out of bounds geometry index for setting number of LOD levels");
        return;
    }

    _geometries[index].Resize(num);
    // Ensure that a _valid geometry object exists at each index
    for (auto it = _geometries[index].Begin(); it != _geometries[index].End(); ++it)
    {
        if (it->IsNull())
            *it = new Geometry();
    }
}

void AModel::SetLocalBoundingBox(const BoundingBoxF& box)
{
    _boundingBox = box;
}

void AModel::SetBones(const TVector<FBone>& bones_, size_t rootBoneIndex)
{
    _bones = bones_;
    _rootBoneIndex = rootBoneIndex;
}

void AModel::SetBoneMappings(const TVector<TVector<size_t> >& boneMappings)
{
    _boneMappings = boneMappings;
}

size_t AModel::GetNumLodLevels(size_t index) const
{
    return index < _geometries.Size() ? _geometries[index].Size() : 0;
}

Geometry* AModel::GetGeometry(size_t index, size_t lodLevel) const
{
    return (index < _geometries.Size() && lodLevel < _geometries[index].Size()) ? _geometries[index][lodLevel].Get() : nullptr;
}

}
