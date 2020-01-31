#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Gameplay/World.h"
#include "IO/JSONFile.h"
#include "Math/BoundingBox.h"
#include "Math/AutoMath.h"
#include "IO/ObjectRef.h"
#include "Debug/Log.h"
#include "Container/Singleton.h"

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
	/// Save node all property to JSON.
	void SavePropertyJSONs(FJSONValue& dest, AActor* node);
	/// Save property to JSON.
	void SavePropertyJSON(FJSONValue& dest, const FProperty& prop, AActor* node);
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

}