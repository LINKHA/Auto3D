#pragma once

#include "Container/AutoPtr.h"
#include "Graphics/GPUObject.h"
#include "Graphics/GraphicsDefs.h"

namespace Auto3D
{

class JSONValue;

/// GPU buffer for shader constant data.
class AUTO_API ConstantBuffer : public FRefCounted, public GPUObject
{
public:
    /// Construct.
    ConstantBuffer();
    /// Destruct.
    ~ConstantBuffer();

    /// Release the buffer.
    void Release() override;
    /// Recreate the GPU resource after data loss.
    void Recreate() override;

    /// Load from JSON data. Return true on success.
    bool LoadJSON(const JSONValue& source);
    /// Save as JSON data.
    void SaveJSON(JSONValue& dest);
    /// Define the constants being used and create the GPU-side buffer. Return true on success.
    bool Define(ResourceUsage::Type usage, const TVector<Constant>& srcConstants);
    /// Define the constants being used and create the GPU-side buffer. Return true on success.
    bool Define(ResourceUsage::Type usage, size_t numConstants, const Constant* srcConstants);
    /// Set a constant by index. Optionally specify how many elements to update, default all. Return true on success.
    bool SetConstant(size_t index, const void* _data, size_t numElements = 0);
    /// Set a constant by name. Optionally specify how many elements to update, default all. Return true on success.
    bool SetConstant(const FString& name, const void* _data, size_t numElements = 0);
    /// Set a constant by name. Optionally specify how many elements to update, default all. Return true on success.
    bool SetConstant(const char* name, const void* _data, size_t numElements = 0);
    /// Apply to the GPU-side buffer if has changes. Can only be used once on an immutable buffer. Return true on success.
    bool Apply();
    /// Set raw data directly to the GPU-side buffer. Optionally copy back to the shadow constants. Return true on success.
    bool SetData(const void* _data, bool copyToShadow = false);
    /// Set a constant by index, template version.
    template <typename _Ty> bool SetConstant(size_t index, const _Ty& _data, size_t numElements = 0) { return SetConstant(index, (const void*)&_data, numElements); }
    /// Set a constant by name, template version.
    template <typename _Ty> bool SetConstant(const FString& name, const _Ty& _data, size_t numElements = 0) { return SetConstant(name, (const void*)&_data, numElements); }
    /// Set a constant by name, template version.
    template <typename _Ty> bool SetConstant(const char* name, const _Ty& _data, size_t numElements = 0) { return SetConstant(name, (const void*)&_data, numElements); }

    /// Return number of constants.
    size_t GetNumConstants() const { return _constants.Size(); }
    /// Return the constant descriptions.
    const TVector<Constant>& GetConstants() const { return _constants; }
    /// Return the index of a constant, or NPOS if not found.
    size_t FindConstantIndex(const FString& name) const;
    /// Return the index of a constant, or NPOS if not found.
    size_t FindConstantIndex(const char* name) const;
    /// Return pointer to the constant value, or null if not found.
    const void* ConstantValue(size_t index, size_t elementIndex = 0) const;
    /// Return pointer to the constant value, or null if not found.
    const void* ConstantValue(const FString& name, size_t elementIndex = 0) const;
    /// Return pointer to the constant value, or null if not found.
    const void* ConstantValue(const char* name, size_t elementIndex = 0) const;

    /// Return constant value, template version.
    template <typename _Ty> _Ty ConstantValue(size_t index, size_t elementIndex = 0) const
    {
        const void* value = ConstantValue(index, elementIndex);
        return value ? *(reinterpret_cast<const _Ty*>(value)) : _Ty();
    }

    /// Return constant value, template version.
    template <typename _Ty> _Ty ConstantValue(const FString& name, size_t elementIndex = 0) const
    {
        const void* value = ConstantValue(name, elementIndex);
        return value ? *(reinterpret_cast<const _Ty*>(value)) : _Ty();
    }

    /// Return constant value, template version.
    template <typename _Ty> _Ty ConstantValue(const char* name, size_t elementIndex = 0) const
    {
        const void* value = ConstantValue(name, elementIndex);
        return value ? *(reinterpret_cast<const _Ty*>(value)) : _Ty();
    }

    /// Return total byte _size of the buffer.
    size_t GetByteSize() const { return _byteSize; }
    /// Return whether buffer has unapplied changes.
    bool IsDirty() const { return _dirty; }
    /// Return resource usage type.
    ResourceUsage::Type GetUsage() const { return _usage; }
    /// Return whether is dynamic.
    bool IsDynamic() const { return _usage == ResourceUsage::DYNAMIC; }
    /// Return whether is immutable.
    bool IsImmutable() const { return _usage == ResourceUsage::IMMUTABLE; }

    /// Return the OpenGL buffer identifier. Used internally and should not be called by portable application code.
    unsigned GetGLBuffer() const { return _buffer; }

    /// Index for "constant not found."
    static const size_t NPOS = (size_t)-1;

private:
    /// Create the GPU-side constant buffer. Called on the first Apply() if the buffer is immutable. Return true on success.
    bool Create(const void* _data = nullptr);

    /// OpenGL buffer object identifier.
    unsigned _buffer;
    /// Constant definitions.
    TVector<Constant> _constants;
    /// CPU-side data where updates are collected before applying.
    TAutoArrayPtr<unsigned char> _shadowData;
    /// Total byte _size.
    size_t _byteSize;
    /// AResource usage type.
    ResourceUsage::Type _usage;
    /// Dirty flag.
    bool _dirty;
};

}