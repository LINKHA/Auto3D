#pragma once

#include "Container/HashMap.h"
#include "Container/String.h"
#include "Container/Vector.h"
#include "Container/Ptr.h"
#include "Math/Vector4.h"


namespace Auto3D
{

class FJSONValue;
class FStream;

typedef TVector<FJSONValue> JSONArray;
typedef THashMap<FString, FJSONValue> JSONObject;

/// JSON value types.
namespace EJSONType
{
	enum Type
	{
		Null = 0,
		BOOL,
		NUMBER,
		VECTOR2,
		VECTOR3,
		VECTOR4,
		STRING,
		ARRAY,
		OBJECT,
		Count
	};
};


/// JSON data union.
struct FJSONData
{
	
    union
    {
        char charValue;
        bool boolValue;
        double numberValue;
        size_t padding[4];
    };
};

/// JSON value. Stores a boolean, string or number, or either an array or dictionary-like collection of nested values.
class AUTO_API FJSONValue
{
    friend class OJSONFile;
    
public:
    /// Construct a null value.
    FJSONValue();
    /// Copy-construct.
    FJSONValue(const FJSONValue& value);
    /// Construct from a boolean.
    FJSONValue(bool value);
    /// Construct from an integer number.
    FJSONValue(int value);
    /// Construct from an unsigned integer number.
    FJSONValue(unsigned value);
    /// Construct from a floating point number.
    FJSONValue(float value);
    /// Construct from a floating point number.
    FJSONValue(double value);
	/// Construct from a vector2
	FJSONValue(const TVector2F& value);
	/// Construct from a vector3
	FJSONValue(const TVector3F& value);
	/// Construct from a vector4
	FJSONValue(const TVector4F& value);
    /// Construct from a string.
    FJSONValue(const FString& value);
    /// Construct from a C string.
    FJSONValue(const char* value);
    /// Construct from a JSON object.
    FJSONValue(const JSONArray& value);
    /// Construct from a JSON object.
    FJSONValue(const JSONObject& value);
    /// Destruct.
    ~FJSONValue();
    
    /// Assign a JSON value.
    FJSONValue& operator = (const FJSONValue& rhs);
    /// Assign a boolean.
    FJSONValue& operator = (bool rhs);
    /// Assign an integer number.
    FJSONValue& operator = (int rhs);
    /// Assign an unsigned integer number.
    FJSONValue& operator = (unsigned rhs);
    /// Assign a floating point number.
    FJSONValue& operator = (float rhs);
    /// Assign a floating point number.
    FJSONValue& operator = (double rhs);
	/// Assign a vetor2
	FJSONValue& operator = (const TVector2F& value);
	/// Assign a vetor3
	FJSONValue& operator = (const TVector3F& value);
	/// Assign a vetor4
	FJSONValue& operator = (const TVector4F& value);
    /// Assign a string.
    FJSONValue& operator = (const FString& value);
    /// Assign a C string.
    FJSONValue& operator = (const char* value);
    /// Assign a JSON array.
    FJSONValue& operator = (const JSONArray& value);
    /// Assign a JSON object.
    FJSONValue& operator = (const JSONObject& value);
    /// Index as an array. Becomes an array if was not before.
    FJSONValue& operator [] (size_t index);
    /// Const index as an array. Return a null value if not an array.
    const FJSONValue& operator [] (size_t index) const;
    /// Index as an object. Becomes an object if was not before.
    FJSONValue& operator [] (const FString& key);
    /// Const index as an object. Return a null value if not an object.
    const FJSONValue& operator [] (const FString& key) const;
    /// Test for equality with another JSON value.
    bool operator == (const FJSONValue& rhs) const;
    /// Test for inequality.
    bool operator != (const FJSONValue& rhs) const { return !(*this == rhs); }
    
    /// Parse from a string. Return true on success.
    bool FromString(const FString& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    /// Parse from a binary stream.
    void FromBinary(FStream& source);
    /// Write to a string. Called recursively to write nested values.
    void ToString(FString& dest, int spacing = 2, int indent = 0) const;
    /// Return as string.
    FString ToString(int spacing = 2) const;
    /// Serialize to a binary stream.
    void ToBinary(FStream& dest) const;
    
    /// Push a value at the end. Becomes an array if was not before.
    void Push(const FJSONValue& value);
    /// Insert a value at _position. Becomes an array if was not before.
    void Insert(size_t index, const FJSONValue& value);
    /// Remove the last value. No-op if not an array.
    void Pop();
    /// Remove indexed value(s). No-op if not an array.
    void Erase(size_t pos, size_t length = 1);
    /// Resize array. Becomes an array if was not before.
    void Resize(size_t newSize);
    /// Insert an associative value. Becomes an object if was not before.
    void Insert(const TPair<FString, FJSONValue>& pair);
    /// Remove an associative value. No-op if not an object.
    void Erase(const FString& key);
    /// Clear array or object. No-op otherwise.
    void Clear();
    /// Set to an empty array.
    void SetEmptyArray();
    /// Set to an empty object.
    void SetEmptyObject();
    /// Set to null value.
    void SetNull();
    
    /// Return number of values for objects or arrays, or 0 otherwise.
    size_t Size() const;
    /// Return whether an object or array is empty. Return false if not an object or array.
    bool IsEmpty() const;
    
    /// Return type.
    EJSONType::Type Type() const { return _type; }
    /// Return whether is null.
    bool IsNull() const { return _type == EJSONType::Null; }
    /// Return whether is a bool.
    bool IsBool() const { return _type == EJSONType::BOOL; }
    /// Return whether is a number.
    bool IsNumber() const { return _type == EJSONType::NUMBER; }
	/// Return whether is a vector2.
	bool IsVector2() const { return _type == EJSONType::VECTOR2; }
	/// Return whether is a vector3.
	bool IsVector3() const { return _type == EJSONType::VECTOR3; }
	/// Return whether is a vector4.
	bool IsVector4() const { return _type == EJSONType::VECTOR4; }
    /// Return whether is a string.
    bool IsString() const { return _type == EJSONType::STRING; }
    /// Return whether is an array.
    bool IsArray() const { return _type == EJSONType::ARRAY; }
    /// Return whether is an object.
    bool IsObject() const { return _type == EJSONType::OBJECT; }
    /// Return value as a bool, or false on type mismatch.
    bool GetBool() const { return _type == EJSONType::BOOL ? _data.boolValue : false; }
    /// Return value as a number, or zero on type mismatch.
    double GetNumber() const { return _type == EJSONType::NUMBER ? _data.numberValue : 0.0; }
	/// Return value as a vector2, or zero on type mismatch.
	const TVector2F& GetVector2() const { return _type == EJSONType::VECTOR2 ? *(reinterpret_cast<const TVector2F*>(&_data)) : TVector2F::ZERO; }
	/// Return value as a vector3, or zero on type mismatch.
	const TVector3F& GetVector3() const { return _type == EJSONType::VECTOR3 ? *(reinterpret_cast<const TVector3F*>(&_data)) : TVector3F::ZERO; }
	/// Return value as a vector4, or zero on type mismatch.
	const TVector4F& GetVector4() const { return _type == EJSONType::VECTOR4 ? *(reinterpret_cast<const TVector4F*>(&_data)) : TVector4F::ZERO; }
    /// Return value as a string, or empty string on type mismatch.
    const FString& GetString() const { return _type == EJSONType::STRING ? *(reinterpret_cast<const FString*>(&_data)) : FString::EMPTY; }
    /// Return value as an array, or empty on type mismatch.
    const JSONArray& GetArray() const { return _type == EJSONType::ARRAY ? *(reinterpret_cast<const JSONArray*>(&_data)) : emptyJSONArray; }
    /// Return value as an object, or empty on type mismatch.
    const JSONObject& GetObject() const { return _type == EJSONType::OBJECT ? *(reinterpret_cast<const JSONObject*>(&_data)) : emptyJSONObject; }

    /// Return whether has an associative value.
    bool Contains(const FString& key) const;
    
    /// Empty (null) value.
    static const FJSONValue EMPTY;
    /// Empty array.
    static const JSONArray emptyJSONArray;
    /// Empty object.
    static const JSONObject emptyJSONObject;
    
private:
    /// Parse from a char buffer. Return true on success.
    bool Parse(const char*&pos, const char*& end);
    /// Assign a new type and perform the necessary dynamic allocation / deletion.
    void SetType(EJSONType::Type newType);
    
    /// Append a string in JSON format into the destination.
    static void WriteJSONString(FString& dest, const FString& str);
    /// Append indent spaces to the destination.
    static void WriteIndent(FString& dest, int indent);
	/// Read a vector2 in JSON format from a stream. Return true on success.
	static bool ReadJSONVector2(TVector2F& dest, const char*& pos, const char*& end);
	/// Read a vector3 in JSON format from a stream. Return true on success.
	static bool ReadJSONVector3(TVector3F& dest, const char*& pos, const char*& end);
	/// Read a vector4 in JSON format from a stream. Return true on success.
	static bool ReadJSONVector4(TVector4F& dest, const char*& pos, const char*& end);
    /// Read a string in JSON format from a stream. Return true on success.
    static bool ReadJSONString(FString& dest, const char*& pos, const char*& end, bool inQuote);
    /// Match until the end of a string. Return true if successfully matched.
    static bool MatchString(const char* str, const char*& pos, const char*& end);
    /// Scan until a character is found. Return true if successfully matched.
    static bool MatchChar(char c, const char*& pos, const char*& end);

    /// Get the next char from a stream. Return true on success or false if the stream ended.
    static bool NextChar(char& dest, const char*& pos, const char*& end, bool skipWhiteSpace)
    {
        while (pos < end)
        {
            dest = *pos;
            ++pos;
            if (!skipWhiteSpace || dest > 0x20)
                return true;
        }
        
        return false;
    }
    
    /// Type.
    EJSONType::Type _type;
    /// Value data.
    FJSONData _data;
};

}
