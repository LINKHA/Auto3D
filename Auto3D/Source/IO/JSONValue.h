#pragma once

#include "Container/HashMap.h"
#include "Container/String.h"
#include "Container/Vector.h"
#include "Container/Ptr.h"
#include "Math/Vector4.h"


namespace Auto3D
{

class JSONValue;
class Stream;

typedef Vector<JSONValue> JSONArray;
typedef HashMap<String, JSONValue> JSONObject;

/// JSON value types.
namespace JSONType
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
struct JSONData
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
class AUTO_API JSONValue
{
    friend class JSONFile;
    
public:
    /// Construct a null value.
    JSONValue();
    /// Copy-construct.
    JSONValue(const JSONValue& value);
    /// Construct from a boolean.
    JSONValue(bool value);
    /// Construct from an integer number.
    JSONValue(int value);
    /// Construct from an unsigned integer number.
    JSONValue(unsigned value);
    /// Construct from a floating point number.
    JSONValue(float value);
    /// Construct from a floating point number.
    JSONValue(double value);
	/// Construct from a vector2
	JSONValue(const Vector2F& value);
	/// Construct from a vector3
	JSONValue(const Vector3F& value);
	/// Construct from a vector4
	JSONValue(const Vector4F& value);
    /// Construct from a string.
    JSONValue(const String& value);
    /// Construct from a C string.
    JSONValue(const char* value);
    /// Construct from a JSON object.
    JSONValue(const JSONArray& value);
    /// Construct from a JSON object.
    JSONValue(const JSONObject& value);
    /// Destruct.
    ~JSONValue();
    
    /// Assign a JSON value.
    JSONValue& operator = (const JSONValue& rhs);
    /// Assign a boolean.
    JSONValue& operator = (bool rhs);
    /// Assign an integer number.
    JSONValue& operator = (int rhs);
    /// Assign an unsigned integer number.
    JSONValue& operator = (unsigned rhs);
    /// Assign a floating point number.
    JSONValue& operator = (float rhs);
    /// Assign a floating point number.
    JSONValue& operator = (double rhs);
	/// Assign a vetor2
	JSONValue& operator = (const Vector2F& value);
	/// Assign a vetor3
	JSONValue& operator = (const Vector3F& value);
	/// Assign a vetor4
	JSONValue& operator = (const Vector4F& value);
    /// Assign a string.
    JSONValue& operator = (const String& value);
    /// Assign a C string.
    JSONValue& operator = (const char* value);
    /// Assign a JSON array.
    JSONValue& operator = (const JSONArray& value);
    /// Assign a JSON object.
    JSONValue& operator = (const JSONObject& value);
    /// Index as an array. Becomes an array if was not before.
    JSONValue& operator [] (size_t index);
    /// Const index as an array. Return a null value if not an array.
    const JSONValue& operator [] (size_t index) const;
    /// Index as an object. Becomes an object if was not before.
    JSONValue& operator [] (const String& key);
    /// Const index as an object. Return a null value if not an object.
    const JSONValue& operator [] (const String& key) const;
    /// Test for equality with another JSON value.
    bool operator == (const JSONValue& rhs) const;
    /// Test for inequality.
    bool operator != (const JSONValue& rhs) const { return !(*this == rhs); }
    
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    /// Parse from a binary stream.
    void FromBinary(Stream& source);
    /// Write to a string. Called recursively to write nested values.
    void ToString(String& dest, int spacing = 2, int indent = 0) const;
    /// Return as string.
    String ToString(int spacing = 2) const;
    /// Serialize to a binary stream.
    void ToBinary(Stream& dest) const;
    
    /// Push a value at the end. Becomes an array if was not before.
    void Push(const JSONValue& value);
    /// Insert a value at _position. Becomes an array if was not before.
    void Insert(size_t index, const JSONValue& value);
    /// Remove the last value. No-op if not an array.
    void Pop();
    /// Remove indexed value(s). No-op if not an array.
    void Erase(size_t pos, size_t length = 1);
    /// Resize array. Becomes an array if was not before.
    void Resize(size_t newSize);
    /// Insert an associative value. Becomes an object if was not before.
    void Insert(const Pair<String, JSONValue>& pair);
    /// Remove an associative value. No-op if not an object.
    void Erase(const String& key);
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
    JSONType::Type Type() const { return _type; }
    /// Return whether is null.
    bool IsNull() const { return _type == JSONType::Null; }
    /// Return whether is a bool.
    bool IsBool() const { return _type == JSONType::BOOL; }
    /// Return whether is a number.
    bool IsNumber() const { return _type == JSONType::NUMBER; }
	/// Return whether is a vector2.
	bool IsVector2() const { return _type == JSONType::VECTOR2; }
	/// Return whether is a vector3.
	bool IsVector3() const { return _type == JSONType::VECTOR3; }
	/// Return whether is a vector4.
	bool IsVector4() const { return _type == JSONType::VECTOR4; }
    /// Return whether is a string.
    bool IsString() const { return _type == JSONType::STRING; }
    /// Return whether is an array.
    bool IsArray() const { return _type == JSONType::ARRAY; }
    /// Return whether is an object.
    bool IsObject() const { return _type == JSONType::OBJECT; }
    /// Return value as a bool, or false on type mismatch.
    bool GetBool() const { return _type == JSONType::BOOL ? _data.boolValue : false; }
    /// Return value as a number, or zero on type mismatch.
    double GetNumber() const { return _type == JSONType::NUMBER ? _data.numberValue : 0.0; }
	/// Return value as a vector2, or zero on type mismatch.
	const Vector2F& GetVector2() const { return _type == JSONType::VECTOR2 ? *(reinterpret_cast<const Vector2F*>(&_data)) : Vector2F::ZERO; }
	/// Return value as a vector3, or zero on type mismatch.
	const Vector3F& GetVector3() const { return _type == JSONType::VECTOR3 ? *(reinterpret_cast<const Vector3F*>(&_data)) : Vector3F::ZERO; }
	/// Return value as a vector4, or zero on type mismatch.
	const Vector4F& GetVector4() const { return _type == JSONType::VECTOR4 ? *(reinterpret_cast<const Vector4F*>(&_data)) : Vector4F::ZERO; }
    /// Return value as a string, or empty string on type mismatch.
    const String& GetString() const { return _type == JSONType::STRING ? *(reinterpret_cast<const String*>(&_data)) : String::EMPTY; }
    /// Return value as an array, or empty on type mismatch.
    const JSONArray& GetArray() const { return _type == JSONType::ARRAY ? *(reinterpret_cast<const JSONArray*>(&_data)) : emptyJSONArray; }
    /// Return value as an object, or empty on type mismatch.
    const JSONObject& GetObject() const { return _type == JSONType::OBJECT ? *(reinterpret_cast<const JSONObject*>(&_data)) : emptyJSONObject; }
    /// Return whether has an associative value.
    bool Contains(const String& key) const;
    
    /// Empty (null) value.
    static const JSONValue EMPTY;
    /// Empty array.
    static const JSONArray emptyJSONArray;
    /// Empty object.
    static const JSONObject emptyJSONObject;
    
private:
    /// Parse from a char buffer. Return true on success.
    bool Parse(const char*&pos, const char*& end);
    /// Assign a new type and perform the necessary dynamic allocation / deletion.
    void SetType(JSONType::Type newType);
    
    /// Append a string in JSON format into the destination.
    static void WriteJSONString(String& dest, const String& str);
    /// Append indent spaces to the destination.
    static void WriteIndent(String& dest, int indent);
	/// Read a vector2 in JSON format from a stream. Return true on success.
	static bool ReadJSONVector2(Vector2F& dest, const char*& pos, const char*& end);
	/// Read a vector3 in JSON format from a stream. Return true on success.
	static bool ReadJSONVector3(Vector3F& dest, const char*& pos, const char*& end);
	/// Read a vector4 in JSON format from a stream. Return true on success.
	static bool ReadJSONVector4(Vector4F& dest, const char*& pos, const char*& end);
    /// Read a string in JSON format from a stream. Return true on success.
    static bool ReadJSONString(String& dest, const char*& pos, const char*& end, bool inQuote);
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
    JSONType::Type _type;
    /// Value data.
    JSONData _data;
};

}
