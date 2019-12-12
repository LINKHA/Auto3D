#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Scene/Node.h"
#include "Resource/JSONFile.h"


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
	FPropertyType(const Type& type)
	{
		SetType(type);
	}

	void SetType(const Type& type)
	{
		_type = EPropertyType::Count;
		FString typeStr = RtToStr(type.get_name());
		if (typeStr == "bool")
		{

		}
		else if (typeStr == "char")
		{

		}
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
		dest["type"] = RtToStr(Type::get(*node).get_name());
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
		Type type = Type::get(*node);

		for (auto& prop : type.get_properties())
		{

			dest = *(reinterpret_cast<const FJSONValue*>(prop));

			//LogString("  name: " + RtToStr(prop.get_name()));
			//LogString("    type: " + RtToStr(prop.get_type().get_name()));
		}
	}
};

}