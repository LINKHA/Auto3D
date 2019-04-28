#include "../Debug/Log.h"
#include "../Debug/Profiler.h"
#include "../IO/JSONValue.h"
#include "../Object/Attribute.h"
#include "ConstantBuffer.h"
#include "GraphicsDefs.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

static const AttributeType::Type elementToAttribute[] =
{
    AttributeType::INT,
    AttributeType::FLOAT,
    AttributeType::VECTOR2,
    AttributeType::VECTOR3,
    AttributeType::VECTOR4,
    AttributeType::Count,
    AttributeType::MATRIX3X4,
    AttributeType::MATRIX4,
    AttributeType::Count
};

bool ConstantBuffer::LoadJSON(const JSONValue& source)
{
    ResourceUsage::Type usage_ = ResourceUsage::DEFAULT;
    if (source.Contains("usage"))
        usage_ = (ResourceUsage::Type)String::ListIndex(source["usage"].GetString(), resourceUsageNames, ResourceUsage::DEFAULT);

    Vector<Constant> constants_;

    const JSONValue& jsonConstants = source["constants"];
    for (size_t i = 0; i < jsonConstants.Size(); ++i)
    {
        const JSONValue& jsonConstant = jsonConstants[i];
        const String& type = jsonConstant["type"].GetString();

        Constant newConstant;
        newConstant._name = jsonConstant["name"].GetString();

        newConstant._type = (ElementType::Type)String::ListIndex(type, elementTypeNames, ElementType::Count);
        if (newConstant._type == ElementType::Count)
        {
            ErrorStringF("Unknown element type %s in constant buffer JSON", type.CString());
            break;
        }
        if (jsonConstant.Contains("numElements"))
            newConstant._numElements = (int)jsonConstant["numElements"].GetNumber();

        constants_.Push(newConstant);
    }

    if (!Define(usage_, constants_))
        return false;

    for (size_t i = 0; i < _constants.Size() && i < jsonConstants.Size(); ++i)
    {
        const JSONValue& value = jsonConstants[i]["value"];
        AttributeType::Type attrType = elementToAttribute[_constants[i]._type];

        if (value.IsArray())
        {
            for (size_t j = 0; j < value.Size(); ++j)
                Attribute::FromJSON(attrType, const_cast<void*>(ConstantValue(i, j)), value[j]);
        }
        else
            Attribute::FromJSON(attrType, const_cast<void*>(ConstantValue(i)), value);
    }

    _dirty = true;
    Apply();
    return true;
}

void ConstantBuffer::SaveJSON(JSONValue& dest)
{
    dest.SetEmptyObject();
    dest["usage"] = resourceUsageNames[_usage];
    dest["constants"].SetEmptyArray();

    for (size_t i = 0; i < _constants.Size(); ++i)
    {
        const Constant& constant = _constants[i];
        AttributeType::Type attrType = elementToAttribute[constant._type];

        JSONValue jsonConstant;
        jsonConstant["name"] = constant._name;
        jsonConstant["type"] = elementTypeNames[constant._type];
        if (constant._numElements != 1)
            jsonConstant["numElements"] = (int)constant._numElements;

        if (constant._numElements == 1)
            Attribute::ToJSON(attrType, jsonConstant["value"], ConstantValue(i));
        else
        {
            jsonConstant["value"].Resize(constant._numElements);
            for (size_t j = 0; j < constant._numElements; ++j)
                Attribute::ToJSON(attrType, jsonConstant["value"][j], ConstantValue(i, j));
        }

        dest["constants"].Push(jsonConstant);
    }
}

bool ConstantBuffer::Define(ResourceUsage::Type usage, const Vector<Constant>& srcConstants)
{
    return Define(usage, srcConstants.Size(), srcConstants.Size() ? &srcConstants[0] : nullptr);
}

bool ConstantBuffer::Define(ResourceUsage::Type usage, size_t numConstants, const Constant* srcConstants)
{
    PROFILE(DefineConstantBuffer);
    
    Release();
    
    if (!numConstants)
    {
        ErrorString("Can not define constant buffer with no constants");
        return false;
    }
    if (usage == ResourceUsage::RENDERTARGET)
    {
        ErrorString("Rendertarget usage is illegal for constant buffers");
        return false;
    }
    
    _constants.Clear();
    _byteSize = 0;
    _usage = usage;
    
    while (numConstants--)
    {
        if (srcConstants->_type == ElementType::UBYTE4)
        {
            ErrorString("UBYTE4 type is not supported in constant buffers");
            _constants.Clear();
            _byteSize = 0;
            return false;
        }

        Constant newConstant;
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

    if (_usage != ResourceUsage::IMMUTABLE)
        return Create();
    else
        return true;
}

bool ConstantBuffer::SetConstant(size_t index, const void* _data, size_t numElements)
{
    if (index >= _constants.Size())
        return false;
    
    const Constant& constant = _constants[index];
    
    if (!numElements || numElements > constant._numElements)
        numElements = constant._numElements;
    
    memcpy(_shadowData.Get() + constant._offset, _data, numElements * constant._elementSize);
    _dirty = true;
    return true;
}

bool ConstantBuffer::SetConstant(const String& name, const void* _data, size_t numElements)
{
    return SetConstant(name.CString(), _data, numElements);
}

bool ConstantBuffer::SetConstant(const char* name, const void* _data, size_t numElements)
{
    for (size_t i = 0; i < _constants.Size(); ++i)
    {
        if (_constants[i]._name == name)
            return SetConstant(i, _data, numElements);
    }
    
    return false;
}

bool ConstantBuffer::Apply()
{
    return _dirty ? SetData(_shadowData.Get()) : true;
}

size_t ConstantBuffer::FindConstantIndex(const String& name) const
{
    return FindConstantIndex(name.CString());
}

size_t ConstantBuffer::FindConstantIndex(const char* name) const
{
    for (size_t i = 0; i < _constants.Size(); ++i)
    {
        if (_constants[i]._name == name)
            return i;
    }

    return NPOS;
}

const void* ConstantBuffer::ConstantValue(size_t index, size_t elementIndex) const
{
    return (index < _constants.Size() && elementIndex < _constants[index]._numElements) ? _shadowData.Get() + 
        _constants[index]._offset + elementIndex * _constants[index]._elementSize : nullptr;
}

const void* ConstantBuffer::ConstantValue(const String& name, size_t elementIndex) const
{
    return ConstantValue(FindConstantIndex(name), elementIndex);
}

const void* ConstantBuffer::ConstantValue(const char* name, size_t elementIndex) const
{
    return ConstantValue(FindConstantIndex(name), elementIndex);
}

}
