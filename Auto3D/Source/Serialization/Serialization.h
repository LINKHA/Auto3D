#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Gameplay/World.h"
#include "IO/JSONFile.h"
#include "Math/BoundingBox.h"
#include "Math/AutoMath.h"
#include "IO/ObjectRef.h"
#include "Debug/Log.h"
#include "Container/Singleton.h"
#include "Component/ActorComponent.h"

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

class AUTO_API GSerializationModule : public FRefCounted
{
	REGISTER_SINGLETON(GSerializationModule)
public:
	/// Construct and register subsystem.
	GSerializationModule():
		_modelMaterialsFlag(false)
	{}
	/// Destruct.
	~GSerializationModule() {}
	
	/// Save all nodes under the scene as a JSON format file.
	bool SaveRootJSON(FStream& dest, AWorld* scene);
	/// Load all nodes under the scene as a JSON format file.
	bool LoadRootJSON(FStream& source, AWorld* scene);
	/// Load all nodes under the scene as a JSON format file.
	bool LoadRootJSON(const FJSONValue& source, AWorld* scene);
	/// Save all nodes under scene as a binary format file.
	bool SaveRoot(FStream& dest, AWorld* scene);
	/// Load all nodes under scene as a binary format file.
	bool LoadRoot(FStream& source, AWorld* scene);
private:
	/// Save node and child to JSON.
	void SaveJSON(FJSONValue& dest, AActor* node);
	/// Save component and child to JSON.
	void SaveComponentJSON(FJSONValue& dest, AActorComponent* comp);
	
	/// Save node all property to JSON.
	void SavePropertyJSONs(FJSONValue& dest, AActor* node);
	/// Save property to JSON.
	template<typename _Ty> void SavePropertyJSON(FJSONValue& dest, const FProperty& prop, _Ty* node);
	/// Load node and child from JSON.
	void LoadJSON(const FJSONValue& source, AActor* node);
	/// Load node all property to JSON.
	void LoadPropertyJSONs(const FJSONValue& source, AActor* node);
	/// Load property to JSON.
	void LoadPropertyJSON(const FJSONValue& source, const FProperty& prop, AActor* node);
	/// Save node and child to binary.
	void Save(FStream& dest, AActor* node);
	/// Save node all property to binary.
	void SavePropertys(FStream& dest, AActor* node);
	/// Save property to binary.
	void SaveProperty(FStream& dest, const FProperty& prop, AActor* node);
	/// Load node and child from binary.
	void Load(FStream& source, AActor* node);
	/// Load node all property to binary.
	void LoadPropertys(FStream& source, AActor* node);
	/// Load property to binary.
	void LoadProperty(FStream& source, const FProperty& prop, AActor* node);

	///The model Materials relation falg citation serialization needs to keep the first model Materials member to record whether a 
	///JSONValue came first for materialsAttr, which becomes false after a single JSONValue is read
	bool _modelMaterialsFlag = false;
	/// Temp resource ref.
	FString _resourceRef;
};

template<typename _Ty> void GSerializationModule::SavePropertyJSON(FJSONValue& dest, const FProperty& prop, _Ty* node)
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

	case EPropertyType::OBJECTREF:
		dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FObjectRef>()._id;
		break;

	case EPropertyType::JSONVALUE:
		dest[RtToStr(prop.get_name())] = prop.get_value(node).get_value<FJSONValue>();
	default:
		break;
	}
}


}