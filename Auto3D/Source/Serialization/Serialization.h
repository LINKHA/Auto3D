#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Scene/Node.h"
#include "Scene/Scene.h"
#include "Resource/JSONFile.h"
#include "Math/BoundingBox.h"
#include "Math/AutoMath.h"
#include "IO/ObjectRef.h"
#include "Debug/Log.h"

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

	void SetType(const FType& type);

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
	FSerializationModule():
		_modelMaterialsFlag(false)
	{}
	/// Destruct.
	~FSerializationModule() {}

	bool SaveRootJSON(FStream& dest, ANode* node);
	void SaveJSON(FJSONValue& dest, ANode* node);
	void SavePropertyJSONs(FJSONValue& dest, ANode* node);
	void SavePropertyJSON(FJSONValue& dest, const FProperty& prop, ANode* node);

	bool LoadRootJSON(FStream& source, AScene* scene);
	bool LoadRootJSON(const FJSONValue& source, AScene* scene);
	void LoadJSON(const FJSONValue& source, ANode* node);
	void LoadPropertyJSONs(const FJSONValue& source, ANode* node);
	void LoadPropertyJSON(const FJSONValue& source, const FProperty& prop, ANode* node);

	bool SaveRoot(FStream& dest, ANode* node)
	{
		dest.WriteFileID("SCNE");
		Save(dest, node);

		return true;
	}

	void Save(FStream& dest, ANode* node)
	{
		// Write type and ID first, followed by attributes and child nodes
		dest.Write(RtToStr(FType::get(*node).get_name()));
		dest.Write(node->Id());

		//Save the properties
		SavePropertys(dest, node);

		dest.WriteVLE(node->NumPersistentChildren());

		for (auto it = node->Children().Begin(); it != node->Children().End(); ++it)
		{
			ANode* child = *it;
			if (!child->IsTemporary())
				Save(dest, child);
		}
	}

	void SavePropertys(FStream& dest, ANode* node)
	{
		FType type = FType::get(*node);

		int serializableSize = 0;
		for (auto& prop : type.get_properties())
		{
			if (prop.get_metadata(SERIALIZABLE))
				serializableSize++;
		}
		dest.WriteVLE(serializableSize);

		for (auto& prop : type.get_properties())
		{
			if (prop.get_metadata(SERIALIZABLE))
				SaveProperty(dest, prop, node);
		}

	}

	void SaveProperty(FStream& dest, const FProperty& prop, ANode* node)
	{
		FType type = prop.get_type();
		FPropertyType propertyType(type);
		
		dest.Write<unsigned char>((unsigned)propertyType._type);
		switch (propertyType._type)
		{
		case EPropertyType::BOOL:
			dest.Write<bool>(prop.get_value(node).get_value<bool>());
			break;

		case EPropertyType::BYTE:
			dest.Write<unsigned char>(prop.get_value(node).get_value<unsigned char>());
			break;

		case EPropertyType::UNSIGNED:
			dest.Write<unsigned>(prop.get_value(node).get_value<unsigned>());
			break;

		case EPropertyType::INT:
			dest.Write<int>(prop.get_value(node).get_value<int>());
			break;

		case EPropertyType::INTVECTOR2:
			dest.Write<FString>(prop.get_value(node).get_value<TVector2I>().ToString());
			break;

		case EPropertyType::INTRECT:
			dest.Write<FString>(prop.get_value(node).get_value<TRectI>().ToString());
			break;

		case EPropertyType::FLOAT:
			dest.Write<float>(prop.get_value(node).get_value<float>());
			break;

		case EPropertyType::VECTOR2:
			dest.Write<FString>(prop.get_value(node).get_value<TVector2F>().ToString());
			break;

		case EPropertyType::VECTOR3:
			dest.Write<FString>(prop.get_value(node).get_value<TVector3F>().ToString());
			break;

		case EPropertyType::VECTOR4:
			dest.Write<FString>(prop.get_value(node).get_value<TVector4F>().ToString());
			break;

		case EPropertyType::QUATERNION:
			dest.Write<FString>(prop.get_value(node).get_value<FQuaternion>().ToString());
			break;

		case EPropertyType::COLOR:
			dest.Write<FString>(prop.get_value(node).get_value<FColor>().ToString());
			break;

		case EPropertyType::RECT:	
			dest.Write<FString>(prop.get_value(node).get_value<TRectF>().ToString());
			break;

		case EPropertyType::MATRIX2:	
			dest.Write<FString>(prop.get_value(node).get_value<TMatrix2x2F>().ToString());
			break;

		case EPropertyType::MATRIX3:	
			dest.Write<FString>(prop.get_value(node).get_value<TMatrix3x3F>().ToString());
			break;

		case EPropertyType::MATRIX3X4:	
			dest.Write<FString>(prop.get_value(node).get_value<TMatrix3x4F>().ToString());
			break;

		case EPropertyType::MATRIX4:	
			dest.Write<FString>(prop.get_value(node).get_value<TMatrix4x4F>().ToString());
			break;

		case EPropertyType::STRING:
			dest.Write<FString>(prop.get_value(node).get_value<FString>());
			break;

		case EPropertyType::RESOURCEREF:	
			dest.Write<FResourceRef>(prop.get_value(node).get_value<FResourceRef>().ToString());
			break;

		case EPropertyType::RESOURCEREFLIST:	
			dest.Write<FResourceRefList>(prop.get_value(node).get_value<FResourceRefList>().ToString());
			break;

		case EAttributeType::OBJECTREF:
			dest.Write<unsigned>(prop.get_value(node).get_value<FObjectRef>()._id);
			break;

		case EAttributeType::JSONVALUE:
			dest.Write<FJSONValue>(prop.get_value(node).get_value<FJSONValue>());
			break;

		default:
			break;
		}
	}

	bool LoadRoot(FStream& source, AScene* scene)
	{
		FString fileId = source.ReadFileID();
		if (fileId != "SCNE")
		{
			ErrorString("File is not a binary scene file");
			return false;
		}

		FString ownType = source.Read<FString>();
		unsigned ownId = source.Read<unsigned>();

		if (ownType != scene->GetTypeName())
		{
			ErrorString("Mismatching type of scene root node in scene file");
			return false;
		}

		scene->Clear();
		Load(source, scene);
		return true;
	}

	void Load(FStream& source, ANode* node)
	{
		// Load property to node
		LoadPropertys(source, node);

		size_t numChildren = source.ReadVLE();

		for (size_t i = 0; i < numChildren; ++i)
		{
			FStringHash childType(source.Read<FString>());
			unsigned childId = source.Read<unsigned>();

			ANode* child = node->CreateChild(childType);
			if (child)
			{
				Load(source, child);
			}
		}
	}

	void LoadPropertys(FStream& source, ANode* node)
	{
		FType type = FType::get(*node);
		
		size_t numAttrs = source.ReadVLE();
		for (auto& prop : type.get_properties())
		{
			if (RtToStr(prop.get_name()) == "materialsAttr" || RtToStr(prop.get_name()) == "modelAttr")
			{
				continue;
			}

			if (prop.get_metadata(SERIALIZABLE))
			{
				FString propertyTypeName = source.Read<FString>();
				LoadProperty(source, prop, node);
			}
		}
	}

	void LoadProperty(FStream& source, const FProperty& prop, ANode* node)
	{
		FType type = prop.get_type();
		FPropertyType propertyType(type);


		switch (propertyType._type)
		{
		case EPropertyType::BOOL:
			prop.set_value(node, source.Read<bool>());
			break;

		case EPropertyType::BYTE:
			prop.set_value(node, (unsigned char)source.Read<unsigned char>());
			break;

		case EPropertyType::UNSIGNED:
			prop.set_value(node, (unsigned)source.Read<unsigned>());
			break;

		case EPropertyType::INT:
			prop.set_value(node, (int)source.Read<int>());
			break;

		case EPropertyType::INTVECTOR2:
		{
			TVector2I vec;
			vec.FromString(source.Read<FString>());
			prop.set_value(node, TVector2I(vec._x, vec._y));
		}
		break;

		case EPropertyType::INTRECT:
		{
			TRectI rect = source.Read<FString>();
			prop.set_value(node, TRectI(rect._min._x, rect._min._y, rect._max._x, rect._max._y));
		}
		break;

		case EPropertyType::FLOAT:
			prop.set_value(node, (float)source.Read<float>());
			break;

		case EPropertyType::VECTOR2:
		{
			TVector2F vec;
			vec.FromString(source.Read<FString>());
			prop.set_value(node, vec);
		}
		break;

		case EPropertyType::VECTOR3:
		{
			TVector3F vec;
			vec.FromString(source.Read<FString>());
			prop.set_value(node, vec);
		}
		break;

		case EPropertyType::VECTOR4:
		{
			TVector4F vec;
			vec.FromString(source.Read<FString>());
			prop.set_value(node, vec);
		}
		break;

		case EPropertyType::QUATERNION:
		{
			TVector4F vec;
			vec.FromString(source.Read<FString>());
			FQuaternion quaternion(vec._x, vec._y, vec._z, vec._w);
			prop.set_value(node, quaternion);
		}
		break;

		case EPropertyType::COLOR:
		{
			FColor color;
			color.FromString(source.Read<FString>());
			prop.set_value(node, color);
		}
		break;

		case EPropertyType::RECT:
		{
			TRectF rect;
			rect.FromString(source.Read<FString>());
			prop.set_value(node, rect);
		}
		break;

		case EPropertyType::MATRIX2:
		{
			TMatrix2x2F matrix;
			matrix.FromString(source.Read<FString>());
			prop.set_value(node, matrix);
		}
		break;

		case EPropertyType::MATRIX3:
		{
			TMatrix3x3F matrix;
			matrix.FromString(source.Read<FString>());
			prop.set_value(node, matrix);
		}
		break;

		case EPropertyType::MATRIX3X4:
		{
			TMatrix3x4F matrix;
			matrix.FromString(source.Read<FString>());
			prop.set_value(node, matrix);
		}
		break;

		case EPropertyType::MATRIX4:
		{
			TMatrix4x4F matrix;
			matrix.FromString(source.Read<FString>());
			prop.set_value(node, matrix);
		}
		break;

		case EPropertyType::STRING:
			prop.set_value(node, source.Read<FString>());
			break;

		case EPropertyType::RESOURCEREF:
		{
			FResourceRef resourceRef;
			resourceRef.FromString(source.Read<FString>());
			prop.set_value(node, resourceRef);
		}
		break;

		case EPropertyType::RESOURCEREFLIST:
		{
			FResourceRefList resourceRef;
			resourceRef.FromString(source.Read<FString>());
			prop.set_value(node, resourceRef);
		}
		break;

		case EAttributeType::OBJECTREF:
		{
			FObjectRef objectRef;
			objectRef._id = (unsigned)source.Read<unsigned>();
			prop.set_value(node, objectRef);
		}
		break;

		case EAttributeType::JSONVALUE:
			prop.set_value(node, source);
		default:
			break;
		}
	}
private:
	///The model Materials relation falg citation serialization needs to keep the first model Materials member to record whether a 
	///JSONValue came first for materialsAttr, which becomes false after a single JSONValue is read
	bool _modelMaterialsFlag = false;
};

}