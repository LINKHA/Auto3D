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
	FSerializationModule();
	/// Destruct.
	~FSerializationModule();

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

		for (auto& prop : type.get_properties())
		{

			dest = *(reinterpret_cast<const FJSONValue*>(prop));

			//LogString("  name: " + RtToStr(prop.get_name()));
			//LogString("    type: " + RtToStr(prop.get_type().get_name()));
		}
	}
};

}