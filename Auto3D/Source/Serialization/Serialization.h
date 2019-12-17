#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Scene/Node.h"
#include "Scene/Scene.h"
#include "Resource/JSONFile.h"
#include "Math/BoundingBox.h"
#include "Math/AutoMath.h"
#include "IO/ObjectRef.h"

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
		else if (type == FType::get<unsigned char>())
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
		else if (type == FType::get<FResourceRef>())
		{
			_type = EPropertyType::RESOURCEREF;
		}
		else if (type == FType::get<FResourceRefList>())
		{
			_type = EPropertyType::RESOURCEREFLIST;
		}
		else if (type == FType::get<FObjectRef>())
		{
			_type = EPropertyType::OBJECTREF;
		}
		else if (type == FType::get<FJSONValue>())
		{
			_type = EPropertyType::JSONVALUE;
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
	FSerializationModule():
		_modelMaterialsFlag(false)
	{}
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
		
		for (auto& prop : type.get_properties())
		{
			if(prop.get_metadata(SERIALIZABLE))
				SaveProperty(dest, prop, node);
		}
	}
	void SaveProperty(FJSONValue& dest, const FProperty& prop, ANode* node)
	{
		FType type = prop.get_type();
		FPropertyType propertyType(type);

		switch (propertyType._type)
		{
		case EPropertyType::BOOL:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<bool>();
			break;

		case EPropertyType::BYTE:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<unsigned char>();
			break;

		case EPropertyType::UNSIGNED:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<unsigned>();
			break;

		case EPropertyType::INT:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<int>();
			break;

		case EPropertyType::INTVECTOR2: 
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TVector2I>().ToString();
			break;

		case EPropertyType::INTRECT:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TRectI>().ToString();
			break;

		case EPropertyType::FLOAT:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<float>();
			break;

		case EPropertyType::VECTOR2:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TVector2F>().ToString();
			break;

		case EPropertyType::VECTOR3:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TVector3F>().ToString();
			break;

		case EPropertyType::VECTOR4:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TVector4F>().ToString();
			break;

		case EPropertyType::QUATERNION:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FQuaternion>().ToString();
			break;

		case EPropertyType::COLOR:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FColor>().ToString();
			break;

		case EPropertyType::RECT:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TRectF>().ToString();
			break;

		case EPropertyType::MATRIX2:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TMatrix2x2F>().ToString();
			break;

		case EPropertyType::MATRIX3:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TMatrix3x3F>().ToString();
			break;

		case EPropertyType::MATRIX3X4:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TMatrix3x4F>().ToString();
			break;

		case EPropertyType::MATRIX4:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TMatrix4x4F>().ToString();
			break;

		case EPropertyType::STRING:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FString>();
			break;

		case EPropertyType::RESOURCEREF:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FResourceRef>().ToString();
			break;

		case EPropertyType::RESOURCEREFLIST:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FResourceRefList>().ToString();
			break;

		case EAttributeType::OBJECTREF:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FObjectRef>()._id;
			break;

		case EAttributeType::JSONVALUE:
			dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FJSONValue>();
		default:
			break;
		}
	}

	bool LoadRootJSON(FStream& source, AScene* scene)
	{
		AJSONFile json;
		bool success = json.Load(source);
		LoadRootJSON(json.Root(), scene);
		return success;
	}

	bool LoadRootJSON(const FJSONValue& source, AScene* scene)
	{
		FString ownType = source["type"].GetString();
		unsigned ownId = (unsigned)source["id"].GetNumber();

		if (ownType != RtToStr(FType::get(*scene).get_name()))
		{
			ErrorString("Mismatching type of scene root node in scene file");
			return false;
		}

		scene->Clear();

		LoadJSON(source, scene);
		return true;
	}

	void LoadJSON(const FJSONValue& source, ANode* node)
	{
		// Load property to node
		LoadPropertyJSON(source, node);

		const JSONArray& children = source["children"].GetArray();
		if (children.Size())
		{
			for (auto it = children.Begin(); it != children.End(); ++it)
			{
				const FJSONValue& childJSON = *it;
				FStringHash childType(childJSON["type"].GetString());
				unsigned childId = (unsigned)childJSON["id"].GetNumber();
				ANode* child = node->CreateChild(childType);
				if (child)
				{
					LoadJSON(childJSON, child);
				}
			}
		}

	}

	void LoadPropertyJSON(const FJSONValue& source, ANode* node)
	{
		FType type = FType::get(*node);
#define _GET_OBJECT GetObject
#undef GetObject
		const JSONObject& object = source.GetObject();
#define GetObject _GET_OBJECT
		for (auto& prop : type.get_properties())
		{
			if (RtToStr(prop.get_name()) == "materialsAttr" || RtToStr(prop.get_name()) == "modelAttr")
			{
				//Materials and models have context that require special treatment
				if (RtToStr(prop.get_name()) == "materialsAttr")
				{
					_modelMaterialsFlag = true;
					continue;
				}
				if (prop.get_metadata(SERIALIZABLE))
				{
					auto jsonIt = object.Find(RtToStr(prop.get_name()));
					if (jsonIt != object.End())
					{
						LoadProperty(jsonIt->_second, prop, node);
					}

				}
				//Materials and models have context that require special treatment
				if (RtToStr(prop.get_name()) == "modelAttr" && _modelMaterialsFlag)
				{
					_modelMaterialsFlag = false;
					auto jsonIt = object.Find("materialsAttr");
					if (jsonIt != object.End())
					{
						for (auto& prop : type.get_properties())
						{
							if (RtToStr(prop.get_name()) == "materialsAttr")
								LoadProperty(jsonIt->_second, prop, node);
						}
					}
				}
				continue;
			}

			if (prop.get_metadata(SERIALIZABLE))
			{
				auto jsonIt = object.Find(RtToStr(prop.get_name()));
				if (jsonIt != object.End())
				{
					LoadProperty(jsonIt->_second, prop, node);
				}

			}

		}
		_modelMaterialsFlag = false;
	}

	void LoadProperty(const FJSONValue& source, const FProperty& prop, ANode* node)
	{
		FType type = prop.get_type();
		FPropertyType propertyType(type);


		switch (propertyType._type)
		{
		case EPropertyType::BOOL:
			prop.set_value(node, source.GetBool());
			break;

		case EPropertyType::BYTE:
			prop.set_value(node, (unsigned char)source.GetNumber());
			break;

		case EPropertyType::UNSIGNED:
			prop.set_value(node, (unsigned)source.GetNumber());
			break;

		case EPropertyType::INT:
			prop.set_value(node, (int)source.GetNumber());
			break;

		case EPropertyType::INTVECTOR2:
			{
				TVector2F vec2 = source.GetVector2();
				prop.set_value(node, TVector2I(vec2._x, vec2._y));
			}
			break;

		case EPropertyType::INTRECT:
			{
				TRectI rect = source.GetString();
				prop.set_value(node, TRectI(rect._min._x, rect._min._y, rect._max._x, rect._max._y));
			}
			break;

		case EPropertyType::FLOAT:
			prop.set_value(node, (float)source.GetNumber());
			break;

		case EPropertyType::VECTOR2:
			{
				TVector2F vec;
				vec.FromString(source.GetString());
				prop.set_value(node, vec);
			}
			break;

		case EPropertyType::VECTOR3:
			{
				TVector3F vec;
				vec.FromString(source.GetString());
				prop.set_value(node, vec);
			}
			break;

		case EPropertyType::VECTOR4:
			{
				TVector4F vec;
				vec.FromString(source.GetString());
				prop.set_value(node, vec);
			}
			break;

		case EPropertyType::QUATERNION:
			{
				TVector4F vec;
				vec.FromString(source.GetString());
				FQuaternion quaternion(vec._x, vec._y, vec._z, vec._w);
				prop.set_value(node, quaternion);
			}
			break;

		case EPropertyType::COLOR:
			{
				FColor color;
				color.FromString(source.GetString());
				prop.set_value(node, color);
			}
			break;

		case EPropertyType::RECT:
			{
				TRectF rect;
				rect.FromString(source.GetString());
				prop.set_value(node, rect);
			}
			break;

		case EPropertyType::MATRIX2:
			{
				TMatrix2x2F matrix;
				matrix.FromString(source.GetString());
				prop.set_value(node, matrix);
			}
			break;

		case EPropertyType::MATRIX3:
			{
				TMatrix3x3F matrix;
				matrix.FromString(source.GetString());
				prop.set_value(node, matrix);
			}
			break;

		case EPropertyType::MATRIX3X4:
			{
				TMatrix3x4F matrix;
				matrix.FromString(source.GetString());
				prop.set_value(node, matrix);
			}
			break;

		case EPropertyType::MATRIX4:
		{
			TMatrix4x4F matrix;
			matrix.FromString(source.GetString());
			prop.set_value(node, matrix);
		}
			break;

		case EPropertyType::STRING:
			prop.set_value(node, source.GetString());
			break;

		case EPropertyType::RESOURCEREF:
		{
			FResourceRef resourceRef;
			resourceRef.FromString(source.GetString());
			prop.set_value(node, resourceRef);
		}
			break;

		case EPropertyType::RESOURCEREFLIST:
		{
			FResourceRefList resourceRef;
			resourceRef.FromString(source.GetString());
			prop.set_value(node, resourceRef);
		}
			break;

		case EAttributeType::OBJECTREF:
			{
				FObjectRef objectRef;
				objectRef._id = (unsigned)source.GetNumber();
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