#include "IO/JSONValue.h"
#include "IO/ObjectRef.h"
#include "IO/ResourceRef.h"
#include "Math/BoundingBox.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "Math/Matrix3x4.h"
#include "Math/Matrix2x2.h"

#include "Attribute.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

const FString FAttribute::typeNames[] =
{
    "bool",
    "byte",
    "unsigned",
    "int",
    "IntVector2",
    "IntRect",
    "float",
    "Vector2",
    "Vector3",
    "Vector4",
    "Quaternion",
    "Color",
    "Rect",
    "BoundingBox",
	"Matrix2",
    "Matrix3",
    "Matrix3x4",
    "Matrix4",
    "String",
    "ResourceRef",
    "ResourceRefList",
    "ObjectRef",
    "JSONValue",
    ""
};

const size_t FAttribute::byteSizes[] =
{
    sizeof(bool),
    sizeof(unsigned char),
    sizeof(unsigned),
    sizeof(int),
    sizeof(TVector2I),
    sizeof(TRectI),
    sizeof(float),
    sizeof(TVector2F),
    sizeof(TVector3F),
    sizeof(TVector4F),
    sizeof(FQuaternion),
    sizeof(FColor),
    sizeof(TRectF),
    sizeof(TBoundingBoxF),
	sizeof(TMatrix2x2F),
    sizeof(TMatrix3x3F),
    sizeof(TMatrix3x4F),
    sizeof(TMatrix4x4F),
    0,
    0,
    0,
    sizeof(unsigned),
    0,
    0
};

IAttributeAccessor::~IAttributeAccessor()
{
}

FAttribute::FAttribute(const char* name, IAttributeAccessor* accessor, const char** enumNames) :
    _name(name),
    _accessor(accessor),
    _enumNames(enumNames)
{
}

void FAttribute::FromValue(ASerializable* instance, const void* source)
{
    _accessor->Set(instance, source);
}

void FAttribute::ToValue(ASerializable* instance, void* dest)
{
    _accessor->Get(instance, dest);
}

void FAttribute::Skip(EAttributeType::Type type, FStream& source)
{
    if (byteSizes[type])
    {
        source.Seek(source.Position() + byteSizes[type]);
        return;
    }

    switch (type)
    {
    case EAttributeType::STRING:
        source.Read<FString>();
        break;

    case EAttributeType::RESOURCEREF:
        source.Read<FResourceRef>();
        break;

    case EAttributeType::RESOURCEREFLIST:
        source.Read<FResourceRefList>();
        break;

    case EAttributeType::OBJECTREF:
        source.Read<FObjectRef>();
        break;

    case EAttributeType::JSONVALUE:
        source.Read<FJSONValue>();
        break;

    default:
        break;
    }
}

const FString& FAttribute::TypeName() const
{
    return typeNames[Type()];
}

size_t FAttribute::ByteSize() const
{
    return byteSizes[Type()];
}

void FAttribute::FromJSON(EAttributeType::Type type, void* dest, const FJSONValue& source)
{
    switch (type)
    {
    case EAttributeType::BOOL:
        *(reinterpret_cast<bool*>(dest)) = source.GetBool();
        break;

    case EAttributeType::BYTE:
        *(reinterpret_cast<unsigned char*>(dest)) = (unsigned char)source.GetNumber();
        break;

    case EAttributeType::UNSIGNED:
        *(reinterpret_cast<unsigned*>(dest)) = (unsigned)source.GetNumber();
        break;

    case EAttributeType::INT:
        *(reinterpret_cast<int*>(dest)) = (int)source.GetNumber();
        break;

    case EAttributeType::INTRECT:
        reinterpret_cast<TRectI*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::FLOAT:
        *(reinterpret_cast<float*>(dest)) = (float)source.GetNumber();
        break;

    case EAttributeType::VECTOR2:
        *reinterpret_cast<TVector2F*>(dest) = source.GetVector2();
        break;

    case EAttributeType::VECTOR3:
		*reinterpret_cast<TVector3F*>(dest) = source.GetVector3();
        break;

    case EAttributeType::VECTOR4:
        *reinterpret_cast<TVector4F*>(dest) = source.GetVector4();
        break;

    case EAttributeType::QUATERNION:
        reinterpret_cast<TVector4F*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::COLOR:
        reinterpret_cast<FColor*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::RECT:
        reinterpret_cast<TRectF*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::BOUNDINGBOX:
        reinterpret_cast<TRectF*>(dest)->FromString(source.GetString());
        break;

	case EAttributeType::MATRIX2:
		reinterpret_cast<TMatrix2x2F*>(dest)->FromString(source.GetString());
		break;

    case EAttributeType::MATRIX3:
        reinterpret_cast<TMatrix3x3F*>(dest)->FromString(source.GetString());
        break;

	case EAttributeType::MATRIX4:
		reinterpret_cast<TMatrix4x4F*>(dest)->FromString(source.GetString());
		break;

    case EAttributeType::MATRIX3X4:
        reinterpret_cast<TMatrix3x4F*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::STRING:
        *(reinterpret_cast<FString*>(dest)) = source.GetString();
        break;

    case EAttributeType::RESOURCEREF:
        reinterpret_cast<FResourceRef*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::RESOURCEREFLIST:
        reinterpret_cast<FResourceRefList*>(dest)->FromString(source.GetString());
        break;

    case EAttributeType::OBJECTREF:
        reinterpret_cast<FObjectRef*>(dest)->_id = (unsigned)source.GetNumber();
        break;

    case EAttributeType::JSONVALUE:
        *(reinterpret_cast<FJSONValue*>(dest)) = source;
        break;

    default:
        break;
    }
}

void FAttribute::ToJSON(EAttributeType::Type type, FJSONValue& dest, const void* source)
{
    switch (type)
    {
    case EAttributeType::BOOL:
        dest = *(reinterpret_cast<const bool*>(source));
        break;

    case EAttributeType::BYTE:
        dest = *(reinterpret_cast<const unsigned char*>(source));
        break;

    case EAttributeType::UNSIGNED:
        dest = *(reinterpret_cast<const unsigned*>(source));
        break;

    case EAttributeType::INT:
        dest = *(reinterpret_cast<const int*>(source));
        break;

    case EAttributeType::INTVECTOR2:
        dest = reinterpret_cast<const TVector2I*>(source)->ToString();
        break;

    case EAttributeType::INTRECT:
        dest = reinterpret_cast<const TRectI*>(source)->ToString();
        break;

    case EAttributeType::FLOAT:
        dest = *(reinterpret_cast<const float*>(source));
        break;

    case EAttributeType::VECTOR2:
        dest = reinterpret_cast<const TVector2<float>*>(source)->ToString();
        break;

    case EAttributeType::VECTOR3:
        dest = reinterpret_cast<const TVector3F*>(source)->ToString();
        break;

    case EAttributeType::VECTOR4:
        dest = reinterpret_cast<const TVector4F*>(source)->ToString();
        break;

    case EAttributeType::QUATERNION:
        dest = reinterpret_cast<const FQuaternion*>(source)->ToString();
        break;

    case EAttributeType::COLOR:
        dest = reinterpret_cast<const FColor*>(source)->ToString();
        break;

    case EAttributeType::RECT:
        dest = reinterpret_cast<const TRectF*>(source)->ToString();
        break;

    case EAttributeType::BOUNDINGBOX:
        dest = reinterpret_cast<const TBoundingBoxF*>(source)->ToString();
        break;

    case EAttributeType::MATRIX3:
        dest = reinterpret_cast<const TMatrix3x3F*>(source)->ToString();
        break;

    case EAttributeType::MATRIX3X4:
        dest = reinterpret_cast<const TMatrix3x4F*>(source)->ToString();
        break;

    case EAttributeType::MATRIX4:
        dest = reinterpret_cast<const TMatrix4x4F*>(source)->ToString();
        break;

    case EAttributeType::STRING:
        dest = *(reinterpret_cast<const FString*>(source));
        break;

    case EAttributeType::RESOURCEREF:
        dest = reinterpret_cast<const FResourceRef*>(source)->ToString();
        break;

    case EAttributeType::RESOURCEREFLIST:
        dest = reinterpret_cast<const FResourceRefList*>(source)->ToString();
        break;

    case EAttributeType::OBJECTREF:
        dest = reinterpret_cast<const FObjectRef*>(source)->_id;
        break;

    case EAttributeType::JSONVALUE:
        dest = *(reinterpret_cast<const FJSONValue*>(source));
        break;

    default:
        break;
    }
}

EAttributeType::Type FAttribute::TypeFromName(const FString& name)
{
    return (EAttributeType::Type)FString::ListIndex(name, &typeNames[0], EAttributeType::Count);
}

EAttributeType::Type FAttribute::TypeFromName(const char* name)
{
    return (EAttributeType::Type)FString::ListIndex(name, &typeNames[0], EAttributeType::Count);
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<bool>::Type() const
{
    return EAttributeType::BOOL;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<int>::Type() const
{
    return EAttributeType::INT;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<unsigned>::Type() const
{
    return EAttributeType::UNSIGNED;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<unsigned char>::Type() const
{
    return EAttributeType::BYTE;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<float>::Type() const
{
    return EAttributeType::FLOAT;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FString>::Type() const
{
    return EAttributeType::STRING;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<TVector2F>::Type() const
{
    return EAttributeType::VECTOR2;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<TVector3F>::Type() const
{
    return EAttributeType::VECTOR3;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<TVector4F>::Type() const
{
    return EAttributeType::VECTOR4;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FQuaternion>::Type() const
{
    return EAttributeType::QUATERNION;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FColor>::Type() const
{
    return EAttributeType::COLOR;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<TBoundingBoxF>::Type() const
{
    return EAttributeType::BOUNDINGBOX;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FResourceRef>::Type() const
{
    return EAttributeType::RESOURCEREF;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FResourceRefList>::Type() const
{
    return EAttributeType::RESOURCEREFLIST;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FObjectRef>::Type() const
{
    return EAttributeType::OBJECTREF;
}

template<> AUTO_API EAttributeType::Type TAttributeImpl<FJSONValue>::Type() const
{
    return EAttributeType::JSONVALUE;
}

}
