#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/JSONValue.h"
#include "Core/Attribute.h"
#include "ConstantBuffer.h"
#include "GraphicsDefs.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

static const EAttributeType::Type elementToAttribute[] =
{
    EAttributeType::INT,
    EAttributeType::FLOAT,
    EAttributeType::VECTOR2,
    EAttributeType::VECTOR3,
    EAttributeType::VECTOR4,
    EAttributeType::Count,
    EAttributeType::MATRIX3X4,
    EAttributeType::MATRIX4,
    EAttributeType::Count
};

bool FConstantBuffer::LoadJSON(const FJSONValue& source)
{
    EResourceUsage::Type usage = EResourceUsage::DEFAULT;
    if (source.Contains("usage"))
        usage = (EResourceUsage::Type)FString::ListIndex(source["usage"].GetString(), resourceUsageNames, EResourceUsage::DEFAULT);

    TVector<FConstant> constants;

    const FJSONValue& jsonConstants = source["constants"];
    for (size_t i = 0; i < jsonConstants.Size(); ++i)
    {
        const FJSONValue& jsonConstant = jsonConstants[i];
        const FString& type = jsonConstant["type"].GetString();

        FConstant newConstant;
        newConstant._name = jsonConstant["name"].GetString();
        newConstant._type = (EElementType::Type)FString::ListIndex(type, elementTypeNames, EElementType::Count);
        if (newConstant._type == EElementType::Count)
        {
            ErrorStringF("Unknown element type %s in constant buffer JSON", type.CString());
            break;
        }
        if (jsonConstant.Contains("numElements"))
            newConstant._numElements = (int)jsonConstant["numElements"].GetNumber();

        constants.Push(newConstant);
    }

    if (!Define(usage, constants))
        return false;

    for (size_t i = 0; i < _constants.Size() && i < jsonConstants.Size(); ++i)
    {
        const FJSONValue& value = jsonConstants[i]["value"];
        EAttributeType::Type attrType = elementToAttribute[_constants[i]._type];

        if (value.IsArray())
        {
            for (size_t j = 0; j < value.Size(); ++j)
                FAttribute::FromJSON(attrType, const_cast<void*>(ConstantValue(i, j)), value[j]);
        }
        else
            FAttribute::FromJSON(attrType, const_cast<void*>(ConstantValue(i)), value);
    }

    _dirty = true;
    Apply();
    return true;
}

void FConstantBuffer::SaveJSON(FJSONValue& dest)
{
    dest.SetEmptyObject();
    dest["usage"] = resourceUsageNames[_usage];
    dest["constants"].SetEmptyArray();

    for (size_t i = 0; i < _constants.Size(); ++i)
    {
        const FConstant& constant = _constants[i];
        EAttributeType::Type attrType = elementToAttribute[constant._type];

        FJSONValue jsonConstant;
        jsonConstant["name"] = constant._name;
        jsonConstant["type"] = elementTypeNames[constant._type];
        if (constant._numElements != 1)
            jsonConstant["numElements"] = (int)constant._numElements;

        if (constant._numElements == 1)
            FAttribute::ToJSON(attrType, jsonConstant["value"], ConstantValue(i));
        else
        {
            jsonConstant["value"].Resize(constant._numElements);
            for (size_t j = 0; j < constant._numElements; ++j)
                FAttribute::ToJSON(attrType, jsonConstant["value"][j], ConstantValue(i, j));
        }

        dest["constants"].Push(jsonConstant);
    }
}

bool FConstantBuffer::Define(EResourceUsage::Type usage, const TVector<FConstant>& srcConstants)
{
    return Define(usage, srcConstants.Size(), srcConstants.Size() ? &srcConstants[0] : nullptr);
}

bool FConstantBuffer::Define(EResourceUsage::Type usage, size_t numConstants, const FConstant* srcConstants)
{
    PROFILE(DefineConstantBuffer);
    
    Release();
    
    if (!numConstants)
    {
        ErrorString("Can not define constant buffer with no constants");
        return false;
    }
    if (usage == EResourceUsage::RENDERTARGET)
    {
        ErrorString("Rendertarget usage is illegal for constant buffers");
        return false;
    }
    
    _constants.Clear();
    _byteSize = 0;
    _usage = usage;
    
    while (numConstants--)
    {
        if (srcConstants->_type == EElementType::UBYTE4)
        {
            ErrorString("UBYTE4 type is not supported in constant buffers");
            _constants.Clear();
            _byteSize = 0;
            return false;
        }

        FConstant newConstant;
        newConstant._type = srcConstants->_type;
        newConstant._name = srcConstants->_name;
        newConstant._numElements = srcConstants->_numElements;
        newConstant._elementSize = elementSizes[newConstant._type];
        // If element crosses 16 byte boundary or is larger than 16 bytes, align to next 16 bytes
        if ((newConstant._elementSize <= 16 && ((_byteSize + newConstant._elementSize - 1) >> 4) != (_byteSize >> 4)) ||
            (newConstant._elementSize > 16 && (_byteSize & 15)))
            _byteSize += 16 - (_byteSize & 15);
        newConstant._offset = _byteSize;
        _constants.Push(newConstant);
        
        _byteSize += newConstant._elementSize * newConstant._numElements;
        ++srcConstants;
    }

    // Align the final buffer _size to a multiple of 16 bytes
    if (_byteSize & 15)
        _byteSize += 16 - (_byteSize & 15);
    
    _shadowData = new unsigned char[_byteSize];

    if (_usage != EResourceUsage::IMMUTABLE)
        return Create();
    else
        return true;
}

bool FConstantBuffer::SetConstant(size_t index, const void* data, size_t numElements)
{
    if (index >= _constants.Size())
        return false;
    
    const FConstant& constant = _constants[index];
    
    if (!numElements || numElements > constant._numElements)
        numElements = constant._numElements;
    
    memcpy(_shadowData.Get() + constant._offset, data, numElements * constant._elementSize);
    _dirty = true;
    return true;
}

bool FConstantBuffer::SetConstant(const FString& name, const void* data, size_t numElements)
{
    return SetConstant(name.CString(), data, numElements);
}

bool FConstantBuffer::SetConstant(const char* name, const void* data, size_t numElements)
{
    for (size_t i = 0; i < _constants.Size(); ++i)
    {
        if (_constants[i]._name == name)
            return SetConstant(i, data, numElements);
    }
    
    return false;
}

bool FConstantBuffer::Apply()
{
    return _dirty ? SetData(_shadowData.Get()) : true;
}

size_t FConstantBuffer::FindConstantIndex(const FString& name) const
{
    return FindConstantIndex(name.CString());
}

size_t FConstantBuffer::FindConstantIndex(const char* name) const
{
    for (size_t i = 0; i < _constants.Size(); ++i)
    {
        if (_constants[i]._name == name)
            return i;
    }

    return NPOS;
}

const void* FConstantBuffer::ConstantValue(size_t index, size_t elementIndex) const
{
    return (index < _constants.Size() && elementIndex < _constants[index]._numElements) ? _shadowData.Get() + 
        _constants[index]._offset + elementIndex * _constants[index]._elementSize : nullptr;
}

const void* FConstantBuffer::ConstantValue(const FString& name, size_t elementIndex) const
{
    return ConstantValue(FindConstantIndex(name), elementIndex);
}

const void* FConstantBuffer::ConstantValue(const char* name, size_t elementIndex) const
{
    return ConstantValue(FindConstantIndex(name), elementIndex);
}

}
