#include "Serialization.h"
#include "Debug/Log.h"
#include "Debug/Profiler.h"

namespace Auto3D
{

void FPropertyType::SetType(const FType& type)
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
	else if (type == FType::get<TBoundingBoxF>())
	{
		_type = EPropertyType::BOUNDINGBOX;
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

bool FSerializationModule::SaveRootJSON(FStream& dest, AScene* scene)
{
	PROFILE(SaveSceneJSON);

	InfoString("Saving scene to " + dest.GetName());

	AJSONFile json;
	SaveJSON(json.Root(), scene);
	return json.Save(dest);
}

bool FSerializationModule::LoadRootJSON(FStream& source, AScene* scene)
{
	PROFILE(LoadSceneJSON);
	InfoString("Loading scene from " + source.GetName());

	AJSONFile json;
	bool success = json.Load(source);
	LoadRootJSON(json.Root(), scene);
	return success;
}

bool FSerializationModule::LoadRootJSON(const FJSONValue& source, AScene* scene)
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

bool FSerializationModule::SaveRoot(FStream& dest, AScene* scene)
{
	PROFILE(SaveScene);

	InfoString("Saving scene to " + dest.GetName());

	dest.WriteFileID("SCNE");
	Save(dest, scene);

	return true;
}

bool FSerializationModule::LoadRoot(FStream& source, AScene* scene)
{
	PROFILE(LoadScene);

	InfoString("Loading scene from " + source.GetName());

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

bool FSerializationModule::SaveRootJSON(FStream& dest, AScene2D* scene)
{
	PROFILE(SaveSceneJSON);

	InfoString("Saving scene to " + dest.GetName());

	AJSONFile json;
	SaveJSON(json.Root(), scene);
	return json.Save(dest);
}

bool FSerializationModule::LoadRootJSON(FStream& source, AScene2D* scene)
{
	PROFILE(LoadSceneJSON);
	InfoString("Loading scene from " + source.GetName());

	AJSONFile json;
	bool success = json.Load(source);
	LoadRootJSON(json.Root(), scene);
	return success;
}

bool FSerializationModule::LoadRootJSON(const FJSONValue& source, AScene2D* scene)
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

bool FSerializationModule::SaveRoot(FStream& dest, AScene2D* scene)
{
	PROFILE(SaveScene);

	InfoString("Saving scene to " + dest.GetName());

	dest.WriteFileID("SCNE");
	Save(dest, scene);

	return true;
}

bool FSerializationModule::LoadRoot(FStream& source, AScene2D* scene)
{
	PROFILE(LoadScene);

	InfoString("Loading scene from " + source.GetName());

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
//////////////////////////////////////////////////////////////////////////
void FSerializationModule::SaveJSON(FJSONValue& dest, ANode* node)
{
	dest["type"] = RtToStr(FType::get(*node).get_name());
	dest["id"] = node->Id();

	//Save the properties
	SavePropertyJSONs(dest, node);

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

void FSerializationModule::SavePropertyJSONs(FJSONValue& dest, ANode* node)
{
	FType type = FType::get(*node);

	for (auto& prop : type.get_properties())
	{
		if (prop.get_metadata(SERIALIZABLE))
			SavePropertyJSON(dest, prop, node);
	}
}
void FSerializationModule::SavePropertyJSON(FJSONValue& dest, const FProperty& prop, ANode* node)
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

	case EPropertyType::BOUNDINGBOX:
		dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TBoundingBoxF>().ToString();
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

void FSerializationModule::LoadJSON(const FJSONValue& source, ANode* node)
{
	// Load property to node
	LoadPropertyJSONs(source, node);

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

void FSerializationModule::LoadPropertyJSONs(const FJSONValue& source, ANode* node)
{
	FType type = FType::get(*node);

#undef GetObject
	const JSONObject& object = source.GetObject();

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
					LoadPropertyJSON(jsonIt->_second, prop, node);
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
							LoadPropertyJSON(jsonIt->_second, prop, node);
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
				LoadPropertyJSON(jsonIt->_second, prop, node);
			}

		}

	}
	_modelMaterialsFlag = false;
}

void FSerializationModule::LoadPropertyJSON(const FJSONValue& source, const FProperty& prop, ANode* node)
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
		TVector2I vec;
		vec.FromString(source.GetString());
		prop.set_value(node, TVector2I(vec._x, vec._y));
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

	case EPropertyType::BOUNDINGBOX:
	{
		TBoundingBoxF box;
		box.FromString(source.GetString());
		prop.set_value(node, box);
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

void FSerializationModule::Save(FStream& dest, ANode* node)
{
	// Write type and ID first, followed by attributes and child nodes
	FString SS = RtToStr(FType::get(*node).get_name());
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

void FSerializationModule::SavePropertys(FStream& dest, ANode* node)
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

void FSerializationModule::SaveProperty(FStream& dest, const FProperty& prop, ANode* node)
{
	FType type = prop.get_type();
	FPropertyType propertyType(type);

	dest.Write<FString>(RtToStr(prop.get_name()));
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

	case EPropertyType::BOUNDINGBOX:
		dest.Write<FString>(prop.get_value(node).get_value<TBoundingBoxF>().ToString());
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
		dest.Write<FString>(prop.get_value(node).get_value<FResourceRef>().ToString());
		break;

	case EPropertyType::RESOURCEREFLIST:
		dest.Write<FString>(prop.get_value(node).get_value<FResourceRefList>().ToString());
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

void FSerializationModule::Load(FStream& source, ANode* node)
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

void FSerializationModule::LoadPropertys(FStream& source, ANode* node)
{
	FType type = FType::get(*node);

	size_t numAttrs = source.ReadVLE();
	for (int i = 0; i < numAttrs; ++i)
	{
		FString propTypeName = source.Read<FString>();
		FProperty prop = type.get_property(ToRtStr(propTypeName));

		if (RtToStr(prop.get_name()) == "materialsAttr" || RtToStr(prop.get_name()) == "modelAttr")
		{
			//Materials and models have context that require special treatment
			if (RtToStr(prop.get_name()) == "materialsAttr")
			{
				_modelMaterialsFlag = true;
				_resourceRef = source.Read<FString>();
				continue;
			}
			if (prop.get_metadata(SERIALIZABLE))
			{
				LoadProperty(source, prop, node);
			}

			//Materials and models have context that require special treatment
			if (RtToStr(prop.get_name()) == "modelAttr" && _modelMaterialsFlag)
			{
				FProperty materialsProp = type.get_property(ToRtStr("materialsAttr"));
				if (materialsProp.get_metadata(SERIALIZABLE))
				{
					FResourceRefList resourceRef;
					resourceRef.FromString(_resourceRef);
					materialsProp.set_value(node, resourceRef);
				}
				_modelMaterialsFlag = false;
				_resourceRef.Clear();
			}
			continue;
		}

		if (prop.get_metadata(SERIALIZABLE))
		{
			LoadProperty(source, prop, node);
		}
	}
}

void FSerializationModule::LoadProperty(FStream& source, const FProperty& prop, ANode* node)
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

	case EPropertyType::BOUNDINGBOX:
	{
		TBoundingBoxF box;
		box.FromString(source.Read<FString>());
		prop.set_value(node, box);
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
		prop.set_value(node, source.Read<FJSONValue>());
	default:
		break;
	}
}
//////////////////////////////////////////////////////////////////////////
void FSerializationModule::SaveJSON(FJSONValue& dest, ANode2D* node)
{
	dest["type"] = RtToStr(FType::get(*node).get_name());
	dest["id"] = node->Id();

	//Save the properties
	SavePropertyJSONs(dest, node);

	if (node->NumPersistentChildren())
	{
		dest["children"].SetEmptyArray();
		auto children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
		{
			ANode2D* child = *it;
			if (!child->IsTemporary())
			{
				FJSONValue childJSON;
				SaveJSON(childJSON, child);
				dest["children"].Push(childJSON);
			}
		}
	}
}

void FSerializationModule::SavePropertyJSONs(FJSONValue& dest, ANode2D* node)
{
	FType type = FType::get(*node);

	for (auto& prop : type.get_properties())
	{
		if (prop.get_metadata(SERIALIZABLE))
			SavePropertyJSON(dest, prop, node);
	}
}
void FSerializationModule::SavePropertyJSON(FJSONValue& dest, const FProperty& prop, ANode2D* node)
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

	case EPropertyType::BOUNDINGBOX:
		dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<TBoundingBoxF>().ToString();
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

void FSerializationModule::LoadJSON(const FJSONValue& source, ANode2D* node)
{
	// Load property to node
	LoadPropertyJSONs(source, node);

	const JSONArray& children = source["children"].GetArray();
	if (children.Size())
	{
		for (auto it = children.Begin(); it != children.End(); ++it)
		{
			const FJSONValue& childJSON = *it;
			FStringHash childType(childJSON["type"].GetString());
			unsigned childId = (unsigned)childJSON["id"].GetNumber();
			ANode2D* child = node->CreateChild(childType);
			if (child)
			{
				LoadJSON(childJSON, child);
			}
		}
	}

}

void FSerializationModule::LoadPropertyJSONs(const FJSONValue& source, ANode2D* node)
{
	FType type = FType::get(*node);

#undef GetObject
	const JSONObject& object = source.GetObject();

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
					LoadPropertyJSON(jsonIt->_second, prop, node);
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
							LoadPropertyJSON(jsonIt->_second, prop, node);
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
				LoadPropertyJSON(jsonIt->_second, prop, node);
			}

		}

	}
	_modelMaterialsFlag = false;
}

void FSerializationModule::LoadPropertyJSON(const FJSONValue& source, const FProperty& prop, ANode2D* node)
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
		TVector2I vec;
		vec.FromString(source.GetString());
		prop.set_value(node, TVector2I(vec._x, vec._y));
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

	case EPropertyType::BOUNDINGBOX:
	{
		TBoundingBoxF box;
		box.FromString(source.GetString());
		prop.set_value(node, box);
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

void FSerializationModule::Save(FStream& dest, ANode2D* node)
{
	// Write type and ID first, followed by attributes and child nodes
	FString SS = RtToStr(FType::get(*node).get_name());
	dest.Write(RtToStr(FType::get(*node).get_name()));
	dest.Write(node->Id());

	//Save the properties
	SavePropertys(dest, node);

	dest.WriteVLE(node->NumPersistentChildren());

	for (auto it = node->Children().Begin(); it != node->Children().End(); ++it)
	{
		ANode2D* child = *it;
		if (!child->IsTemporary())
			Save(dest, child);
	}
}

void FSerializationModule::SavePropertys(FStream& dest, ANode2D* node)
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

void FSerializationModule::SaveProperty(FStream& dest, const FProperty& prop, ANode2D* node)
{
	FType type = prop.get_type();
	FPropertyType propertyType(type);

	dest.Write<FString>(RtToStr(prop.get_name()));
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

	case EPropertyType::BOUNDINGBOX:
		dest.Write<FString>(prop.get_value(node).get_value<TBoundingBoxF>().ToString());
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
		dest.Write<FString>(prop.get_value(node).get_value<FResourceRef>().ToString());
		break;

	case EPropertyType::RESOURCEREFLIST:
		dest.Write<FString>(prop.get_value(node).get_value<FResourceRefList>().ToString());
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

void FSerializationModule::Load(FStream& source, ANode2D* node)
{
	// Load property to node
	LoadPropertys(source, node);

	size_t numChildren = source.ReadVLE();

	for (size_t i = 0; i < numChildren; ++i)
	{
		FStringHash childType(source.Read<FString>());
		unsigned childId = source.Read<unsigned>();

		ANode2D* child = node->CreateChild(childType);
		if (child)
		{
			Load(source, child);
		}
	}
}

void FSerializationModule::LoadPropertys(FStream& source, ANode2D* node)
{
	FType type = FType::get(*node);

	size_t numAttrs = source.ReadVLE();
	for (int i = 0; i < numAttrs; ++i)
	{
		FString propTypeName = source.Read<FString>();
		FProperty prop = type.get_property(ToRtStr(propTypeName));

		if (RtToStr(prop.get_name()) == "materialsAttr" || RtToStr(prop.get_name()) == "modelAttr")
		{
			//Materials and models have context that require special treatment
			if (RtToStr(prop.get_name()) == "materialsAttr")
			{
				_modelMaterialsFlag = true;
				_resourceRef = source.Read<FString>();
				continue;
			}
			if (prop.get_metadata(SERIALIZABLE))
			{
				LoadProperty(source, prop, node);
			}

			//Materials and models have context that require special treatment
			if (RtToStr(prop.get_name()) == "modelAttr" && _modelMaterialsFlag)
			{
				FProperty materialsProp = type.get_property(ToRtStr("materialsAttr"));
				if (materialsProp.get_metadata(SERIALIZABLE))
				{
					FResourceRefList resourceRef;
					resourceRef.FromString(_resourceRef);
					materialsProp.set_value(node, resourceRef);
				}
				_modelMaterialsFlag = false;
				_resourceRef.Clear();
			}
			continue;
		}

		if (prop.get_metadata(SERIALIZABLE))
		{
			LoadProperty(source, prop, node);
		}
	}
}

void FSerializationModule::LoadProperty(FStream& source, const FProperty& prop, ANode2D* node)
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

	case EPropertyType::BOUNDINGBOX:
	{
		TBoundingBoxF box;
		box.FromString(source.Read<FString>());
		prop.set_value(node, box);
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
		prop.set_value(node, source.Read<FJSONValue>());
	default:
		break;
	}
}

}