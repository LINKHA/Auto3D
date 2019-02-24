#include "../IO/JSONValue.h"
#include "../IO/ObjectRef.h"
#include "../IO/ResourceRef.h"
#include "../Math/BoundingBox.h"
#include "../Math/Color.h"
#include "../Math/IntRect.h"
#include "../Math/Matrix3x4.h"
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
    sizeof(IntVector2),
    sizeof(IntRect),
    sizeof(float),
    sizeof(Vector2),
    sizeof(Vector3),
    sizeof(Vector4),
    sizeof(Quaternion),
    sizeof(Color),
    sizeof(Rect),
    sizeof(BoundingBox),
    sizeof(Matrix3),
    sizeof(Matrix3x4),
    sizeof(Matrix4),
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

Attribute::Attribute(const char* name_, AttributeAccessor* accessor_, const char** enumNames_) :
    name(name_),
    accessor(accessor_),
    enumNames(enumNames_)
{
}

void Attribute::FromValue(Serializable* instance, const void* source)
{
    accessor->Set(instance, source);
}

void Attribute::ToValue(Serializable* instance, void* dest)
{
    accessor->Get(instance, dest);
}

void Attribute::Skip(AttributeType type, Stream& source)
{
    if (byteSizes[type])
    {
        source.Seek(source.Position() + byteSizes[type]);
        return;
    }

    switch (type)
    {
    case ATTR_STRING:
        source.Read<String>();
        break;

    case ATTR_RESOURCEREF:
        source.Read<ResourceRef>();
        break;

    case ATTR_RESOURCEREFLIST:
        source.Read<ResourceRefList>();
        break;

    case ATTR_OBJECTREF:
        source.Read<ObjectRef>();
        break;

    case ATTR_JSONVALUE:
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

void Attribute::FromJSON(AttributeType type, void* dest, const JSONValue& source)
{
    switch (type)
    {
    case ATTR_BOOL:
        *(reinterpret_cast<bool*>(dest)) = source.GetBool();
        break;

    case ATTR_BYTE:
        *(reinterpret_cast<unsigned char*>(dest)) = (unsigned char)source.GetNumber();
        break;

    case ATTR_UNSIGNED:
        *(reinterpret_cast<unsigned*>(dest)) = (unsigned)source.GetNumber();
        break;

    case ATTR_INT:
        *(reinterpret_cast<int*>(dest)) = (int)source.GetNumber();
        break;

    case ATTR_INTVECTOR2:
        reinterpret_cast<IntVector2*>(dest)->FromString(source.GetString());
        break;

    case ATTR_INTRECT:
        reinterpret_cast<IntRect*>(dest)->FromString(source.GetString());
        break;

    case ATTR_FLOAT:
        *(reinterpret_cast<float*>(dest)) = (float)source.GetNumber();
        break;

    case ATTR_VECTOR2:
        reinterpret_cast<Vector2*>(dest)->FromString(source.GetString());
        break;

    case ATTR_VECTOR3:
        reinterpret_cast<Vector3*>(dest)->FromString(source.GetString());
        break;

    case ATTR_VECTOR4:
        reinterpret_cast<Vector4*>(dest)->FromString(source.GetString());
        break;

    case ATTR_QUATERNION:
        reinterpret_cast<Vector4*>(dest)->FromString(source.GetString());
        break;

    case ATTR_COLOR:
        reinterpret_cast<Color*>(dest)->FromString(source.GetString());
        break;

    case ATTR_RECT:
        reinterpret_cast<Rect*>(dest)->FromString(source.GetString());
        break;

    case ATTR_BOUNDINGBOX:
        reinterpret_cast<Rect*>(dest)->FromString(source.GetString());
        break;

    case ATTR_MATRIX3:
        reinterpret_cast<Matrix3*>(dest)->FromString(source.GetString());
        break;

    case ATTR_MATRIX3X4:
        reinterpret_cast<Matrix3x4*>(dest)->FromString(source.GetString());
        break;

    case ATTR_MATRIX4:
        reinterpret_cast<Matrix4*>(dest)->FromString(source.GetString());
        break;

    case ATTR_STRING:
        *(reinterpret_cast<String*>(dest)) = source.GetString();
        break;

    case ATTR_RESOURCEREF:
        reinterpret_cast<ResourceRef*>(dest)->FromString(source.GetString());
        break;

    case ATTR_RESOURCEREFLIST:
        reinterpret_cast<ResourceRefList*>(dest)->FromString(source.GetString());
        break;

    case ATTR_OBJECTREF:
        reinterpret_cast<ObjectRef*>(dest)->id = (unsigned)source.GetNumber();
        break;

    case ATTR_JSONVALUE:
        *(reinterpret_cast<JSONValue*>(dest)) = source;
        break;

    default:
        break;
    }
}

void Attribute::ToJSON(AttributeType type, JSONValue& dest, const void* source)
{
    switch (type)
    {
    case ATTR_BOOL:
        dest = *(reinterpret_cast<const bool*>(source));
        break;

    case ATTR_BYTE:
        dest = *(reinterpret_cast<const unsigned char*>(source));
        break;

    case ATTR_UNSIGNED:
        dest = *(reinterpret_cast<const unsigned*>(source));
        break;

    case ATTR_INT:
        dest = *(reinterpret_cast<const int*>(source));
        break;

    case ATTR_INTVECTOR2:
        dest = reinterpret_cast<const IntVector2*>(source)->ToString();
        break;

    case ATTR_INTRECT:
        dest = reinterpret_cast<const IntRect*>(source)->ToString();
        break;

    case ATTR_FLOAT:
        dest = *(reinterpret_cast<const float*>(source));
        break;

    case ATTR_VECTOR2:
        dest = reinterpret_cast<const Vector2*>(source)->ToString();
        break;

    case ATTR_VECTOR3:
        dest = reinterpret_cast<const Vector3*>(source)->ToString();
        break;

    case ATTR_VECTOR4:
        dest = reinterpret_cast<const Vector4*>(source)->ToString();
        break;

    case ATTR_QUATERNION:
        dest = reinterpret_cast<const Quaternion*>(source)->ToString();
        break;

    case ATTR_COLOR:
        dest = reinterpret_cast<const Color*>(source)->ToString();
        break;

    case ATTR_RECT:
        dest = reinterpret_cast<const Rect*>(source)->ToString();
        break;

    case ATTR_BOUNDINGBOX:
        dest = reinterpret_cast<const BoundingBox*>(source)->ToString();
        break;

    case ATTR_MATRIX3:
        dest = reinterpret_cast<const Matrix3*>(source)->ToString();
        break;

    case ATTR_MATRIX3X4:
        dest = reinterpret_cast<const Matrix3x4*>(source)->ToString();
        break;

    case ATTR_MATRIX4:
        dest = reinterpret_cast<const Matrix4*>(source)->ToString();
        break;

    case ATTR_STRING:
        dest = *(reinterpret_cast<const String*>(source));
        break;

    case ATTR_RESOURCEREF:
        dest = reinterpret_cast<const ResourceRef*>(source)->ToString();
        break;

    case ATTR_RESOURCEREFLIST:
        dest = reinterpret_cast<const ResourceRefList*>(source)->ToString();
        break;

    case ATTR_OBJECTREF:
        dest = reinterpret_cast<const ObjectRef*>(source)->id;
        break;

    case ATTR_JSONVALUE:
        dest = *(reinterpret_cast<const JSONValue*>(source));
        break;

    default:
        break;
    }
}

AttributeType Attribute::TypeFromName(const String& name)
{
    return (AttributeType)String::ListIndex(name, &typeNames[0], MAX_ATTR_TYPES);
}

AttributeType Attribute::TypeFromName(const char* name)
{
    return (AttributeType)String::ListIndex(name, &typeNames[0], MAX_ATTR_TYPES);
}

template<> AUTO_API AttributeType AttributeImpl<bool>::Type() const
{
    return ATTR_BOOL;
}

template<> AUTO_API AttributeType AttributeImpl<int>::Type() const
{
    return ATTR_INT;
}

template<> AUTO_API AttributeType AttributeImpl<unsigned>::Type() const
{
    return ATTR_UNSIGNED;
}

template<> AUTO_API AttributeType AttributeImpl<unsigned char>::Type() const
{
    return ATTR_BYTE;
}

template<> AUTO_API AttributeType AttributeImpl<float>::Type() const
{
    return ATTR_FLOAT;
}

template<> AUTO_API AttributeType AttributeImpl<String>::Type() const
{
    return ATTR_STRING;
}

template<> AUTO_API AttributeType AttributeImpl<Vector2>::Type() const
{
    return ATTR_VECTOR2;
}

template<> AUTO_API AttributeType AttributeImpl<Vector3>::Type() const
{
    return ATTR_VECTOR3;
}

template<> AUTO_API AttributeType AttributeImpl<Vector4>::Type() const
{
    return ATTR_VECTOR4;
}

template<> AUTO_API AttributeType AttributeImpl<Quaternion>::Type() const
{
    return ATTR_QUATERNION;
}

template<> AUTO_API AttributeType AttributeImpl<Color>::Type() const
{
    return ATTR_COLOR;
}

template<> AUTO_API AttributeType AttributeImpl<BoundingBox>::Type() const
{
    return ATTR_BOUNDINGBOX;
}

template<> AUTO_API AttributeType AttributeImpl<ResourceRef>::Type() const
{
    return ATTR_RESOURCEREF;
}

template<> AUTO_API AttributeType AttributeImpl<ResourceRefList>::Type() const
{
    return ATTR_RESOURCEREFLIST;
}

template<> AUTO_API AttributeType AttributeImpl<ObjectRef>::Type() const
{
    return ATTR_OBJECTREF;
}

template<> AUTO_API AttributeType AttributeImpl<JSONValue>::Type() const
{
    return ATTR_JSONVALUE;
}

}
