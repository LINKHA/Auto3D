#include "AutoConfig.h"
#include "IO/JSONValue.h"
#include "Scene/Node.h"
#include "Scene/Scene.h"
#include "Resource/JSONFile.h"
#include "Math/BoundingBox.h"
#include "Math/AutoMath.h"
#include "IO/ObjectRef.h"
#include "Debug/Log.h"
#include "Auto2D/Scene2D.h"

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
	
	/// Save all nodes under the scene as a JSON format file.
	bool SaveRootJSON(FStream& dest, AScene* scene);
	/// Load all nodes under the scene as a JSON format file.
	bool LoadRootJSON(FStream& source, AScene* scene);
	/// Load all nodes under the scene as a JSON format file.
	bool LoadRootJSON(const FJSONValue& source, AScene* scene);
	/// Save all nodes under scene as a binary format file.
	bool SaveRoot(FStream& dest, AScene* scene);
	/// Load all nodes under scene as a binary format file.
	bool LoadRoot(FStream& source, AScene* scene);

	/// Save all nodes under the scene as a JSON format file.
	bool SaveRootJSON(FStream& dest, AScene2D* scene);
	/// Load all nodes under the scene as a JSON format file.
	bool LoadRootJSON(FStream& source, AScene2D* scene);
	/// Load all nodes under the scene as a JSON format file.
	bool LoadRootJSON(const FJSONValue& source, AScene2D* scene);
	/// Save all nodes under scene as a binary format file.
	bool SaveRoot(FStream& dest, AScene2D* scene);
	/// Load all nodes under scene as a binary format file.
	bool LoadRoot(FStream& source, AScene2D* scene);
private:
	/// Save node and child to JSON.
	void SaveJSON(FJSONValue& dest, ANode* node);
	/// Save node all property to JSON.
	void SavePropertyJSONs(FJSONValue& dest, ANode* node);
	/// Save property to JSON.
	void SavePropertyJSON(FJSONValue& dest, const FProperty& prop, ANode* node);
	/// Load node and child from JSON.
	void LoadJSON(const FJSONValue& source, ANode* node);
	/// Load node all property to JSON.
	void LoadPropertyJSONs(const FJSONValue& source, ANode* node);
	/// Load property to JSON.
	void LoadPropertyJSON(const FJSONValue& source, const FProperty& prop, ANode* node);
	/// Save node and child to binary.
	void Save(FStream& dest, ANode* node);
	/// Save node all property to binary.
	void SavePropertys(FStream& dest, ANode* node);
	/// Save property to binary.
	void SaveProperty(FStream& dest, const FProperty& prop, ANode* node);
	/// Load node and child from binary.
	void Load(FStream& source, ANode* node);
	/// Load node all property to binary.
	void LoadPropertys(FStream& source, ANode* node);
	/// Load property to binary.
	void LoadProperty(FStream& source, const FProperty& prop, ANode* node);

	/// Save node and child to JSON.
	void SaveJSON(FJSONValue& dest, ANode2D* node);
	/// Save node all property to JSON.
	void SavePropertyJSONs(FJSONValue& dest, ANode2D* node);
	/// Save property to JSON.
	void SavePropertyJSON(FJSONValue& dest, const FProperty& prop, ANode2D* node);
	/// Load node and child from JSON.
	void LoadJSON(const FJSONValue& source, ANode2D* node);
	/// Load node all property to JSON.
	void LoadPropertyJSONs(const FJSONValue& source, ANode2D* node);
	/// Load property to JSON.
	void LoadPropertyJSON(const FJSONValue& source, const FProperty& prop, ANode2D* node);
	/// Save node and child to binary.
	void Save(FStream& dest, ANode2D* node);
	/// Save node all property to binary.
	void SavePropertys(FStream& dest, ANode2D* node);
	/// Save property to binary.
	void SaveProperty(FStream& dest, const FProperty& prop, ANode2D* node);
	/// Load node and child from binary.
	void Load(FStream& source, ANode2D* node);
	/// Load node all property to binary.
	void LoadPropertys(FStream& source, ANode2D* node);
	/// Load property to binary.
	void LoadProperty(FStream& source, const FProperty& prop, ANode2D* node);

	///The model Materials relation falg citation serialization needs to keep the first model Materials member to record whether a 
	///JSONValue came first for materialsAttr, which becomes false after a single JSONValue is read
	bool _modelMaterialsFlag = false;
	/// Temp resource ref.
	FString _resourceRef;
};

}