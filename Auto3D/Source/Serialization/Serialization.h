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