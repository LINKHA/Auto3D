#include "../IO/JSONValue.h"
#include "../IO/ObjectRef.h"
#include "../IO/ResourceRef.h"
#include "../Math/BoundingBox.h"
#include "../Math/Color.h"
#include "../Math/Rect.h"
#include "../Math/Matrix3x4.h"
#include "../Math/Matrix2x2.h"

#include "Attribute.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const String Attribute::typeNames[] =
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

const size_t Attribute::byteSizes[] =
{
    sizeof(bool),
    sizeof(unsigned char),
    sizeof(unsigned),
    sizeof(int),
    sizeof(Vector2I),
    sizeof(RectI),
    sizeof(float),
    sizeof(Vector2F),
    sizeof(Vector3F),
    sizeof(Vector4F),
    sizeof(Quaternion),
    sizeof(Color),
    sizeof(RectF),
    sizeof(BoundingBoxF),
	sizeof(Matrix2x2F),
    sizeof(Matrix3x3F),
    sizeof(Matrix3x4F),
    sizeof(Matrix4x4F),
    0,
    0,
    0,
    sizeof(unsigned),
    0,
    0
};

AttributeAccessor::~AttributeAccessor()
{
}

Attribute::Attribute(const char* name, AttributeAccessor* accessor, const char** enumNames) :
    _name(name),
    _accessor(accessor),
    _enumNames(enumNames)
{
}

void Attribute::FromValue(Serializable* instance, const void* source)
{
    _accessor->Set(instance, source);
}

void Attribute::ToValue(Serializable* instance, void* dest)
{
    _accessor->Get(instance, dest);
}

void Attribute::Skip(AttributeType::Type type, Stream& source)
{
    if (byteSizes[type])
    {
        source.Seek(source.Position() + byteSizes[type]);
        return;
    }

    switch (type)
    {
    case AttributeType::STRING:
        source.Read<String>();
        break;

    case AttributeType::RESOURCEREF:
        source.Read<ResourceRef>();
        break;

    case AttributeType::RESOURCEREFLIST:
        source.Read<ResourceRefList>();
        break;

    case AttributeType::OBJECTREF:
        source.Read<ObjectRef>();
        break;

    case AttributeType::JSONVALUE:
        source.Read<JSONValue>();
        break;

    default:
        break;
    }
}

const String& Attribute::TypeName() const
{
    return typeNames[Type()];
}

size_t Attribute::ByteSize() const
{
    return byteSizes[Type()];
}

void Attribute::FromJSON(AttributeType::Type type, void* dest, const JSONValue& source)
{
    switch (type)
    {
    case AttributeType::BOOL:
        *(reinterpret_cast<bool*>(dest)) = source.GetBool();
        break;

    case AttributeType::BYTE:
        *(reinterpret_cast<unsigned char*>(dest)) = (unsigned char)source.GetNumber();
        break;

    case AttributeType::UNSIGNED:
        *(reinterpret_cast<unsigned*>(dest)) = (unsigned)source.GetNumber();
        break;

    case AttributeType::INT:
        *(reinterpret_cast<int*>(dest)) = (int)source.GetNumber();
        break;

    case AttributeType::INTVECTOR2:
        reinterpret_cast<Vector2I*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::INTRECT:
        reinterpret_cast<RectI*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::FLOAT:
        *(reinterpret_cast<float*>(dest)) = (float)source.GetNumber();
        break;

    case AttributeType::VECTOR2:
        reinterpret_cast<Vector2F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::VECTOR3:
        reinterpret_cast<Vector3F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::VECTOR4:
        reinterpret_cast<Vector4F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::QUATERNION:
        reinterpret_cast<Vector4F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::COLOR:
        reinterpret_cast<Color*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::RECT:
        reinterpret_cast<RectF*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::BOUNDINGBOX:
        reinterpret_cast<RectF*>(dest)->FromString(source.GetString());
        break;

	case AttributeType::MATRIX2:
		reinterpret_cast<Matrix2x2F*>(dest)->FromString(source.GetString());
		break;

    case AttributeType::MATRIX3:
        reinterpret_cast<Matrix3x3F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::MATRIX3X4:
        reinterpret_cast<Matrix3x4F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::MATRIX4:
        reinterpret_cast<Matrix4x4F*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::STRING:
        *(reinterpret_cast<String*>(dest)) = source.GetString();
        break;

    case AttributeType::RESOURCEREF:
        reinterpret_cast<ResourceRef*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::RESOURCEREFLIST:
        reinterpret_cast<ResourceRefList*>(dest)->FromString(source.GetString());
        break;

    case AttributeType::OBJECTREF:
        reinterpret_cast<ObjectRef*>(dest)->_id = (unsigned)source.GetNumber();
        break;

    case AttributeType::JSONVALUE:
        *(reinterpret_cast<JSONValue*>(dest)) = source;
        break;

    default:
        break;
    }
}

void Attribute::ToJSON(AttributeType::Type type, JSONValue& dest, const void* source)
{
    switch (type)
    {
    case AttributeType::BOOL:
        dest = *(reinterpret_cast<const bool*>(source));
        break;

    case AttributeType::BYTE:
        dest = *(reinterpret_cast<const unsigned char*>(source));
        break;

    case AttributeType::UNSIGNED:
        dest = *(reinterpret_cast<const unsigned*>(source));
        break;

    case AttributeType::INT:
        dest = *(reinterpret_cast<const int*>(source));
        break;

    case AttributeType::INTVECTOR2:
        dest = reinterpret_cast<const Vector2I*>(source)->ToString();
        break;

    case AttributeType::INTRECT:
        dest = reinterpret_cast<const RectI*>(source)->ToString();
        break;

    case AttributeType::FLOAT:
        dest = *(reinterpret_cast<const float*>(source));
        break;

    case AttributeType::VECTOR2:
        dest = reinterpret_cast<const Vector2<float>*>(source)->ToString();
        break;

    case AttributeType::VECTOR3:
        dest = reinterpret_cast<const Vector3F*>(source)->ToString();
        break;

    case AttributeType::VECTOR4:
        dest = reinterpret_cast<const Vector4F*>(source)->ToString();
        break;

    case AttributeType::QUATERNION:
        dest = reinterpret_cast<const Quaternion*>(source)->ToString();
        break;

    case AttributeType::COLOR:
        dest = reinterpret_cast<const Color*>(source)->ToString();
        break;

    case AttributeType::RECT:
        dest = reinterpret_cast<const RectF*>(source)->ToString();
        break;

    case AttributeType::BOUNDINGBOX:
        dest = reinterpret_cast<const BoundingBoxF*>(source)->ToString();
        break;

    case AttributeType::MATRIX3:
        dest = reinterpret_cast<const Matrix3x3F*>(source)->ToString();
        break;

    case AttributeType::MATRIX3X4:
        dest = reinterpret_cast<const Matrix3x4F*>(source)->ToString();
        break;

    case AttributeType::MATRIX4:
        dest = reinterpret_cast<const Matrix4x4F*>(source)->ToString();
        break;

    case AttributeType::STRING:
        dest = *(reinterpret_cast<const String*>(source));
        break;

    case AttributeType::RESOURCEREF:
        dest = reinterpret_cast<const ResourceRef*>(source)->ToString();
        break;

    case AttributeType::RESOURCEREFLIST:
        dest = reinterpret_cast<const ResourceRefList*>(source)->ToString();
        break;

    case AttributeType::OBJECTREF:
        dest = reinterpret_cast<const ObjectRef*>(source)->_id;
        break;

    case AttributeType::JSONVALUE:
        dest = *(reinterpret_cast<const JSONValue*>(source));
        break;

    default:
        break;
    }
}

AttributeType::Type Attribute::TypeFromName(const String& name)
{
    return (AttributeType::Type)String::ListIndex(name, &typeNames[0], AttributeType::Count);
}

AttributeType::Type Attribute::TypeFromName(const char* name)
{
    return (AttributeType::Type)String::ListIndex(name, &typeNames[0], AttributeType::Count);
}

template<> AUTO_API AttributeType::Type AttributeImpl<bool>::Type() const
{
    return AttributeType::BOOL;
}

template<> AUTO_API AttributeType::Type AttributeImpl<int>::Type() const
{
    return AttributeType::INT;
}

template<> AUTO_API AttributeType::Type AttributeImpl<unsigned>::Type() const
{
    return AttributeType::UNSIGNED;
}

template<> AUTO_API AttributeType::Type AttributeImpl<unsigned char>::Type() const
{
    return AttributeType::BYTE;
}

template<> AUTO_API AttributeType::Type AttributeImpl<float>::Type() const
{
    return AttributeType::FLOAT;
}

template<> AUTO_API AttributeType::Type AttributeImpl<String>::Type() const
{
    return AttributeType::STRING;
}

template<> AUTO_API AttributeType::Type AttributeImpl<Vector2F>::Type() const
{
    return AttributeType::VECTOR2;
}

template<> AUTO_API AttributeType::Type AttributeImpl<Vector3F>::Type() const
{
    return AttributeType::VECTOR3;
}

template<> AUTO_API AttributeType::Type AttributeImpl<Vector4F>::Type() const
{
    return AttributeType::VECTOR4;
}

template<> AUTO_API AttributeType::Type AttributeImpl<Quaternion>::Type() const
{
    return AttributeType::QUATERNION;
}

template<> AUTO_API AttributeType::Type AttributeImpl<Color>::Type() const
{
    return AttributeType::COLOR;
}

template<> AUTO_API AttributeType::Type AttributeImpl<BoundingBoxF>::Type() const
{
    return AttributeType::BOUNDINGBOX;
}

template<> AUTO_API AttributeType::Type AttributeImpl<ResourceRef>::Type() const
{
    return AttributeType::RESOURCEREF;
}

template<> AUTO_API AttributeType::Type AttributeImpl<ResourceRefList>::Type() const
{
    return AttributeType::RESOURCEREFLIST;
}

template<> AUTO_API AttributeType::Type AttributeImpl<ObjectRef>::Type() const
{
    return AttributeType::OBJECTREF;
}

template<> AUTO_API AttributeType::Type AttributeImpl<JSONValue>::Type() const
{
    return AttributeType::JSONVALUE;
}

}
