#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Scene/Node.h"
#include "Resource/JSONFile.h"
#include "Math/BoundingBox.h"
#include "Math/AutoMath.h"

namespace Auto3D
{

/// Supported pavePropertyJSON types.
namespace EPropertyType
{
	enum Type
	{
		BOOL = 0,
		BYTE,
		UNSIGNED,
		INT,
		INTVECTOR2,
		INTRECT, 
		FLOAT,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		QUATERNION,
		COLOR,
		RECT,
		BOUNDINGBOX,
		MATRIX2,
		MATRIX3,
		MATRIX3X4,
		MATRIX4,
		STRING,
		RESOURCEREF,
		RESOURCEREFLIST,
		OBJECTREF,
		JSONVALUE,
		Count
	};
};

struct FPropertyType
{
	FPropertyType(const FType& type)
	{
		SetType(type);
	}

	void SetType(const FType& type)
	{
		_type = EPropertyType::Count;

		if (type == FType::get<bool>())
		{
			_type = EPropertyType::BOOL;
		}
		else if (type == FType::get<char>())
		{
			_type = EPropertyType::BYTE;
		}
		else if (type == FType::get<unsigned>())
		{
			_type = EPropertyType::UNSIGNED;
		}
		else if (type == FType::get<int>())
		{
			_type = EPropertyType::INT;
		}
		else if (type == FType::get<TVector2I>())
		{
			_type = EPropertyType::INTVECTOR2;
		}
		else if (type == FType::get<TRectI>())
		{
			_type = EPropertyType::INTRECT;
		}
		else if (type == FType::get<float>())
		{
			_type = EPropertyType::FLOAT;
		}
		else if (type == FType::get<TVector2F>())
		{
			_type = EPropertyType::VECTOR2;
		}
		else if (type == FType::get<TVector3F>())
		{
			_type = EPropertyType::VECTOR3;
		}
		else if (type == FType::get<TVector4F>())
		{
			_type = EPropertyType::VECTOR4;
		}
		else if (type == FType::get<FQuaternion>())
		{
			_type = EPropertyType::QUATERNION;
		}
		else if (type == FType::get<FColor>())
		{
			_type = EPropertyType::COLOR;
		}
		else if (type == FType::get<TRectF>())
		{
			_type = EPropertyType::RECT;
		}
		else if (type == FType::get<TMatrix2x2F>())
		{
			_type = EPropertyType::MATRIX2;
		}
		else if (type == FType::get<TMatrix3x3F>())
		{
			_type = EPropertyType::MATRIX3;
		}
		else if (type == FType::get<TMatrix3x4F>())
		{
			_type = EPropertyType::MATRIX3X4;
		}
		else if (type == FType::get<TMatrix4x4F>())
		{
			_type = EPropertyType::MATRIX4;
		}
		else if (type == FType::get<FString>())
		{
			_type = EPropertyType::STRING;
		}

		/*RESOURCEREF,
		RESOURCEREFLIST,
		OBJECTREF,
		JSONVALUE,*/

	}

	EPropertyType::Type	Type()
	{
		return _type;
	}

	EPropertyType::Type _type;
};

class AUTO_API FSerializationModule : public FRefCounted
{
public:
	/// Construct and register subsystem.
	FSerializationModule() {}
	/// Destruct.
	~FSerializationModule() {}

	bool SaveRootJSON(FStream& dest,ANode* node)
	{
		AJSONFile json;
		SaveJSON(json.Root(), node);
		return json.Save(dest);
	}

	void SaveJSON(FJSONValue& dest, ANode* node)
	{
		dest["type"] = RtToStr(FType::get(*node).get_name());
		dest["id"] = node->Id();

		//Save the properties
		SavePropertyJSON(dest, node);

		if (node->NumPersistentChildren())
		{
			dest["children"].SetEmptyArray();
			auto children = node->Children();
			for (auto it = children.Begin(); it != children.End(); ++it)
			{
				ANode* child = *it;
				if (!child->IsTemporary())
				{
					FJSONValue childJSON;
					SaveJSON(childJSON, child);
					dest["children"].Push(childJSON);
				}
			}
		}
	}
	void SavePropertyJSON(FJSONValue& dest, ANode* node)
	{
		FType type = FType::get(*node);
		
		FString ss = RtToStr(type.get_name());

		int sizes = type.get_properties().size();

		for (auto& prop : type.get_properties())
		{
			//prop.get_value(node).get_value<>();

			SetProperty(dest[RtToStr(prop.get_name())], prop, node);

			//LogString("  name: " + RtToStr(prop.get_name()));
			//LogString("    type: " + RtToStr(prop.get_type().get_name()));
		}
	}
	void SetProperty(FJSONValue& dest, const FProperty& prop, ANode* node)
	{
		FType type = prop.get_type();
		FPropertyType propertyType(type);

		switch (propertyType._type)
		{
		case EPropertyType::BOOL:
			dest = prop.get_value(node).get_value<bool>();
			break;

		case EPropertyType::BYTE:
			dest = prop.get_value(node).get_value<char>();
			break;

		case EPropertyType::UNSIGNED:
			dest = prop.get_value(node).get_value<unsigned>();
			break;

		case EPropertyType::INT:
			dest = prop.get_value(node).get_value<int>();
			break;

		case EPropertyType::INTVECTOR2: 
			dest = prop.get_value(node).get_value<TVector2I>().ToString();
			break;

		case EPropertyType::INTRECT:
			dest = prop.get_value(node).get_value<TRectI>().ToString();
			break;

		case EPropertyType::FLOAT:
			dest = prop.get_value(node).get_value<float>();
			break;

		case EPropertyType::VECTOR2:
			dest = prop.get_value(node).get_value<TVector2F>().ToString();
			break;

		case EPropertyType::VECTOR3:
			dest = prop.get_value(node).get_value<TVector3F>().ToString();
			break;

		case EPropertyType::VECTOR4:
			dest = prop.get_value(node).get_value<TVector4F>().ToString();
			break;

		case EPropertyType::QUATERNION:
			dest = prop.get_value(node).get_value<FQuaternion>().ToString();
			break;

		case EPropertyType::COLOR:
			dest = prop.get_value(node).get_value<FColor>().ToString();
			break;

		case EPropertyType::RECT:
			dest = prop.get_value(node).get_value<TRectF>().ToString();
			break;

		case EPropertyType::MATRIX2:
			dest = prop.get_value(node).get_value<TMatrix2x2F>().ToString();
			break;

		case EPropertyType::MATRIX3:
			dest = prop.get_value(node).get_value<TMatrix3x3F>().ToString();
			break;

		case EPropertyType::MATRIX3X4:
			dest = prop.get_value(node).get_value<TMatrix3x4F>().ToString();
			break;

		case EPropertyType::MATRIX4:
			dest = prop.get_value(node).get_value<TMatrix4x4F>().ToString();
			break;

		case EPropertyType::STRING:
			dest = prop.get_value(node).get_value<FString>();
			break;
		default:
			break;
			/*RESOURCEREF,
			RESOURCEREFLIST,
			OBJECTREF,
			JSONVALUE,*/
		}
	}
};

}