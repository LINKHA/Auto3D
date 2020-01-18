#include "Container/Vector.h"
#include "Container/HashMap.h"
#include "Math/Vector4.h"

#include "JSONValue.h"
#include "Stream.h"

#include <cstdio>
#include <cstdlib>

namespace Auto3D
{

const FJSONValue FJSONValue::EMPTY;
const JSONArray FJSONValue::emptyJSONArray;
const JSONObject FJSONValue::emptyJSONObject;

FJSONValue::FJSONValue() :
    _type(EJSONType::Null)
{
}

FJSONValue::FJSONValue(const FJSONValue& value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(bool value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(int value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(unsigned value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(float value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(double value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(const TVector2F& value) :
	_type(EJSONType::Null)
{
	*this = value;
}

FJSONValue::FJSONValue(const TVector3F& value) :
	_type(EJSONType::Null)
{
	*this = value;
}

FJSONValue::FJSONValue(const TVector4F& value) :
	_type(EJSONType::Null)
{
	*this = value;
}

FJSONValue::FJSONValue(const FString& value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(const char* value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(const JSONArray& value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::FJSONValue(const JSONObject& value) :
    _type(EJSONType::Null)
{
    *this = value;
}

FJSONValue::~FJSONValue()
{
    SetType(EJSONType::Null);
}

FJSONValue& FJSONValue::operator = (const FJSONValue& rhs)
{
    SetType(rhs._type);
    
    switch (_type)
    {
    case EJSONType::BOOL:
        _data.boolValue = rhs._data.boolValue;
        break;
        
    case EJSONType::NUMBER:
        _data.numberValue = rhs._data.numberValue;
        break;
	case EJSONType::VECTOR2:
		*(reinterpret_cast<TVector2F*>(&_data)) = *(reinterpret_cast<const TVector2F*>(&rhs._data));
		break;
	case EJSONType::VECTOR3:
		*(reinterpret_cast<TVector3F*>(&_data)) = *(reinterpret_cast<const TVector3F*>(&rhs._data));
		break;
	case EJSONType::VECTOR4:
		*(reinterpret_cast<TVector4F*>(&_data)) = *(reinterpret_cast<const TVector4F*>(&rhs._data));
		break;
    case EJSONType::STRING:
        *(reinterpret_cast<FString*>(&_data)) = *(reinterpret_cast<const FString*>(&rhs._data));
        break;
        
    case EJSONType::ARRAY:
        *(reinterpret_cast<JSONArray*>(&_data)) = *(reinterpret_cast<const JSONArray*>(&rhs._data));
        break;
        
    case EJSONType::OBJECT:
        *(reinterpret_cast<JSONObject*>(&_data)) = *(reinterpret_cast<const JSONObject*>(&rhs._data));
        break;
        
    default:
        break;
    }
    
    return *this;
}

FJSONValue& FJSONValue::operator = (bool rhs)
{
    SetType(EJSONType::BOOL);
    _data.boolValue = rhs;
    return *this;
}

FJSONValue& FJSONValue::operator = (int rhs)
{
    SetType(EJSONType::NUMBER);
    _data.numberValue = (double)rhs;
    return *this;
}

FJSONValue& FJSONValue::operator = (unsigned rhs)
{
    SetType(EJSONType::NUMBER);
    _data.numberValue = (double)rhs;
    return *this;
}

FJSONValue& FJSONValue::operator = (float rhs)
{
    SetType(EJSONType::NUMBER);
    _data.numberValue = (double)rhs;
    return *this;
}

FJSONValue& FJSONValue::operator = (double rhs)
{
    SetType(EJSONType::NUMBER);
    _data.numberValue = rhs;
    return *this;
}

FJSONValue& FJSONValue::operator = (const TVector2F& value)
{
	SetType(EJSONType::VECTOR2);
	*(reinterpret_cast<TVector2F*>(&_data)) = value;
	return *this;
}

FJSONValue& FJSONValue::operator = (const TVector3F& value)
{
	SetType(EJSONType::VECTOR3);
	*(reinterpret_cast<TVector3F*>(&_data)) = value;
	return *this;
}

FJSONValue& FJSONValue::operator = (const TVector4F& value)
{
	SetType(EJSONType::VECTOR4);
	*(reinterpret_cast<TVector4F*>(&_data)) = value;
	return *this;
}

FJSONValue& FJSONValue::operator = (const FString& value)
{
    SetType(EJSONType::STRING);
    *(reinterpret_cast<FString*>(&_data)) = value;
    return *this;
}

FJSONValue& FJSONValue::operator = (const char* value)
{
    SetType(EJSONType::STRING);
    *(reinterpret_cast<FString*>(&_data)) = value;
    return *this;
}

FJSONValue& FJSONValue::operator = (const JSONArray& value)
{
    SetType(EJSONType::ARRAY);
    *(reinterpret_cast<JSONArray*>(&_data)) = value;
    return *this;
}

FJSONValue& FJSONValue::operator = (const JSONObject& value)
{
    SetType(EJSONType::OBJECT);
    *(reinterpret_cast<JSONObject*>(&_data)) = value;
    return *this;
}

FJSONValue& FJSONValue::operator [] (size_t index)
{
    if (_type != EJSONType::ARRAY)
        SetType(EJSONType::ARRAY);
    
    return (*(reinterpret_cast<JSONArray*>(&_data)))[index];
}

const FJSONValue& FJSONValue::operator [] (size_t index) const
{
    if (_type == EJSONType::ARRAY)
        return (*(reinterpret_cast<const JSONArray*>(&_data)))[index];
    else
        return EMPTY;
}

FJSONValue& FJSONValue::operator [] (const FString& key)
{
    if (_type != EJSONType::OBJECT)
        SetType(EJSONType::OBJECT);
    
    return (*(reinterpret_cast<JSONObject*>(&_data)))[key];
}

const FJSONValue& FJSONValue::operator [] (const FString& key) const
{
    if (_type == EJSONType::OBJECT)
    {
        const JSONObject& object = *(reinterpret_cast<const JSONObject*>(&_data));
        auto it = object.Find(key);
        return it != object.End() ? it->_second : EMPTY;
    }
    else
        return EMPTY;
}

bool FJSONValue::operator == (const FJSONValue& rhs) const
{
    if (_type != rhs._type)
        return false;
    
    switch (_type)
    {
    case EJSONType::BOOL:
        return _data.boolValue == rhs._data.boolValue;
        
    case EJSONType::NUMBER:
        return _data.numberValue == rhs._data.numberValue;
	case EJSONType::VECTOR2:
		return *(reinterpret_cast<const TVector2F*>(&_data)) == *(reinterpret_cast<const TVector2F*>(&rhs._data));
	
	case EJSONType::VECTOR3:
		return *(reinterpret_cast<const TVector3F*>(&_data)) == *(reinterpret_cast<const TVector3F*>(&rhs._data));
	
	case EJSONType::VECTOR4:
		return *(reinterpret_cast<const TVector4F*>(&_data)) == *(reinterpret_cast<const TVector4F*>(&rhs._data));

    case EJSONType::STRING:
        return *(reinterpret_cast<const FString*>(&_data)) == *(reinterpret_cast<const FString*>(&rhs._data));
        
    case EJSONType::ARRAY:
        return *(reinterpret_cast<const JSONArray*>(&_data)) == *(reinterpret_cast<const JSONArray*>(&rhs._data));
        
    case EJSONType::OBJECT:
        return *(reinterpret_cast<const JSONObject*>(&_data)) == *(reinterpret_cast<const JSONObject*>(&rhs._data));
        
    default:
        return true;
    }
}

bool FJSONValue::FromString(const FString& str)
{
    const char* pos = str.CString();
    const char* end = pos + str.Length();
    return Parse(pos, end);
}

bool FJSONValue::FromString(const char* str)
{
    const char* pos = str;
    const char* end = pos + FString::CStringLength(str);
    return Parse(pos, end);
}

void FJSONValue::FromBinary(FStream& source)
{
    EJSONType::Type newType = (EJSONType::Type)source.Read<unsigned char>();

    switch (newType)
    {
    case EJSONType::Null:
        Clear();
        break;

    case EJSONType::BOOL:
        *this = source.Read<bool>();
        break;

    case EJSONType::NUMBER:
        *this = source.Read<double>();
        break;

    case EJSONType::STRING:
        *this = source.Read<FString>();
        break;

    case EJSONType::ARRAY:
        {
            SetEmptyArray();
            size_t num = source.ReadVLE();
            for (size_t i = 0; i < num && !source.IsEof(); ++i)
                Push(source.Read<FJSONValue>());
        }
        break;

    case EJSONType::OBJECT:
        {
            SetEmptyObject();
            size_t num = source.ReadVLE();
            for (size_t i = 0; i < num && !source.IsEof(); ++i)
            {
                FString key = source.Read<FString>();
                (*this)[key] = source.Read<FJSONValue>();
            }
        }
        break;

    default:
        break;
    }
}

void FJSONValue::ToString(FString& dest, int spacing, int indent) const
{
    switch (_type)
    {
    case EJSONType::BOOL:
        dest += _data.boolValue;
        return;
        
    case EJSONType::NUMBER:
	{
		// It is prevented from exceeding the maximum range due to the accuracy problem
		if (_data.numberValue > 4.29496e9)
		{
			dest += FString(_data.numberValue);
			return;
		}

        dest += _data.numberValue;
        return;
	}
    case EJSONType::STRING:
        WriteJSONString(dest, *(reinterpret_cast<const FString*>(&_data)));
        return;
        
    case EJSONType::ARRAY:
        {
            const JSONArray& array = GetArray();
            dest += '[';
            
            if (array.Size())
            {
                indent += spacing;
                for (auto it = array.Begin(); it < array.End(); ++it)
                {
                    if (it != array.Begin())
                        dest += ',';
                    dest += '\n';
                    WriteIndent(dest, indent);
                    it->ToString(dest, spacing, indent);
                }
                indent -= spacing;
                dest += '\n';
                WriteIndent(dest, indent);
            }
            
            dest += ']';
        }
        break;
        
    case EJSONType::OBJECT:
        {
            const JSONObject& object = GetObject();
            dest += '{';
            
            if (object.Size())
            {
                indent += spacing;
                for (auto it = object.Begin(); it != object.End(); ++it)
                {
                    if (it != object.Begin())
                        dest += ',';
                    dest += '\n';
                    WriteIndent(dest, indent);
                    WriteJSONString(dest, it->_first);
                    dest += ": ";
                    it->_second.ToString(dest, spacing, indent);
                }
                indent -= spacing;
                dest += '\n';
                WriteIndent(dest, indent);
            }
            
            dest += '}';
        }
        break;
        
    default:
        dest += "null";
    }
}

FString FJSONValue::ToString(int spacing) const
{
    FString ret;
    ToString(ret, spacing);
    return ret;
}

void FJSONValue::ToBinary(FStream& dest) const
{
    dest.Write((unsigned char)_type);

    switch (_type)
    {
    case EJSONType::BOOL:
        dest.Write(_data.boolValue);
        break;

    case EJSONType::NUMBER:
        dest.Write(_data.numberValue);
        break;

    case EJSONType::STRING:
        dest.Write(GetString());
        break;

    case EJSONType::ARRAY:
        {
            const JSONArray& array = GetArray();
            dest.WriteVLE(array.Size());
            for (auto it = array.Begin(); it != array.End(); ++it)
                it->ToBinary(dest);
        }
        break;

    case EJSONType::OBJECT:
        {
            const JSONObject& object = GetObject();
            dest.WriteVLE(object.Size());
            for (auto it = object.Begin(); it != object.End(); ++it)
            {
                dest.Write(it->_first);
                it->_second.ToBinary(dest);
            }
        }
        break;

    default:
        break;
    }
}

void FJSONValue::Push(const FJSONValue& value)
{
    SetType(EJSONType::ARRAY);
    (*(reinterpret_cast<JSONArray*>(&_data))).Push(value);
}

void FJSONValue::Insert(size_t index, const FJSONValue& value)
{
    SetType(EJSONType::ARRAY);
    (*(reinterpret_cast<JSONArray*>(&_data))).Insert(index, value);
}

void FJSONValue::Pop()
{
    if (_type == EJSONType::ARRAY)
        (*(reinterpret_cast<JSONArray*>(&_data))).Pop();
}

void FJSONValue::Erase(size_t pos, size_t length)
{
    if (_type == EJSONType::ARRAY)
        (*(reinterpret_cast<JSONArray*>(&_data))).Erase(pos, length);
}

void FJSONValue::Resize(size_t newSize)
{
    SetType(EJSONType::ARRAY);
    (*(reinterpret_cast<JSONArray*>(&_data))).Resize(newSize);
}

void FJSONValue::Insert(const TPair<FString, FJSONValue>& pair)
{
    SetType(EJSONType::OBJECT);
    (*(reinterpret_cast<JSONObject*>(&_data))).Insert(pair);
}

void FJSONValue::Erase(const FString& key)
{
    if (_type == EJSONType::OBJECT)
        (*(reinterpret_cast<JSONObject*>(&_data))).Erase(key);
}

void FJSONValue::Clear()
{
    if (_type == EJSONType::ARRAY)
        (*(reinterpret_cast<JSONArray*>(&_data))).Clear();
    else if (_type == EJSONType::OBJECT)
        (*(reinterpret_cast<JSONObject*>(&_data))).Clear();
}

void FJSONValue::SetEmptyArray()
{
    SetType(EJSONType::ARRAY);
    Clear();
}

void FJSONValue::SetEmptyObject()
{
    SetType(EJSONType::OBJECT);
    Clear();
}

void FJSONValue::SetNull()
{
    SetType(EJSONType::Null);
}

size_t FJSONValue::Size() const
{
    if (_type == EJSONType::ARRAY)
        return (*(reinterpret_cast<const JSONArray*>(&_data))).Size();
    else if (_type == EJSONType::OBJECT)
        return (*(reinterpret_cast<const JSONObject*>(&_data))).Size();
    else
        return 0;
}

bool FJSONValue::IsEmpty() const
{
    if (_type == EJSONType::ARRAY)
        return (*(reinterpret_cast<const JSONArray*>(&_data))).IsEmpty();
    else if (_type == EJSONType::OBJECT)
        return (*(reinterpret_cast<const JSONObject*>(&_data))).IsEmpty();
    else
        return false;
}

bool FJSONValue::Contains(const FString& key) const
{
    if (_type == EJSONType::OBJECT)
        return (*(reinterpret_cast<const JSONObject*>(&_data))).Contains(key);
    else
        return false;
}

bool FJSONValue::Parse(const char*& pos, const char*& end)
{
    char c;

    // Handle comments
    for (;;)
    {
        if (!NextChar(c, pos, end, true))
            return false;

        if (c == '/')
        {
            if (!NextChar(c, pos, end, false))
                return false;
            if (c == '/')
            {
                // Skip until end of line
                if (!MatchChar('\n', pos, end))
                    return false;
            }
            else if (c == '*')
            {
                // Skip until end of comment
                if (!MatchChar('*', pos, end))
                    return false;
                if (!MatchChar('/', pos, end))
                    return false;
            }
            else
                return false;
        }
        else
            break;
    }

    if (c == '}' || c == ']')
        return false;
	else if (c == 'v')	//vec3
	{
		if (MatchString("ec", pos, end))
		{
			if (MatchString("2", pos, end))
			{
				SetType(EJSONType::VECTOR2);
				return ReadJSONVector2(*(reinterpret_cast<TVector2F*>(&_data)), pos, end);
			}
			else if (MatchString("3", pos, end))
			{
				SetType(EJSONType::VECTOR3);
				return ReadJSONVector3(*(reinterpret_cast<TVector3F*>(&_data)), pos, end);
			}	
			else if (MatchString("4", pos, end))
			{
				SetType(EJSONType::VECTOR4);
				return ReadJSONVector4(*(reinterpret_cast<TVector4F*>(&_data)), pos, end);
			}
		}	
		else
			return false;
	}
    else if (c == 'n')	//null
    {
        SetNull();
        return MatchString("ull", pos, end);
    }
    else if (c == 'f')	//false
    {
        *this = false;
        return MatchString("alse", pos, end);
    }
    else if (c == 't')	//true
    {
        *this = true;
        return MatchString("rue", pos, end);
    }
    else if (IsDigit(c) || c == '-')	//digit
    {
        --pos;
        *this = strtod(pos, const_cast<char**>(&pos));
        return true;
    }
    else if (c == '\"')
    {
        SetType(EJSONType::STRING);
        return ReadJSONString(*(reinterpret_cast<FString*>(&_data)), pos, end, true);
    }
    else if (c == '[')
    {
        SetEmptyArray();
        // Check for empty first
        if (!NextChar(c, pos, end, true))
            return false;
        if (c == ']')
            return true;
        else
            --pos;
        
        for (;;)
        {
            FJSONValue arrayValue;
            if (!arrayValue.Parse(pos, end))
                return false;
            Push(arrayValue);
            if (!NextChar(c, pos, end, true))
                return false;
            if (c == ']')
                break;
            else if (c != ',')
                return false;
        }
        return true;
    }
    else if (c == '{')
    {
        SetEmptyObject();
        if (!NextChar(c, pos, end, true))
            return false;
        if (c == '}')
            return true;
        else
            --pos;
        
        for (;;)
        {
            FString key;
            if (!ReadJSONString(key, pos, end, false))
                return false;
            if (!NextChar(c, pos, end, true))
                return false;
            if (c != ':')
                return false;
            
            FJSONValue objectValue;
            if (!objectValue.Parse(pos, end))
                return false;
            (*this)[key] = objectValue;
            if (!NextChar(c, pos, end, true))
                return false;
            if (c == '}')
                break;
            else if (c != ',')
                return false;
        }
        return true;
    }
    
    return false;
}

void FJSONValue::SetType(EJSONType::Type newType)
{
    if (_type == newType)
        return;
    
    switch (_type)
    {
	case EJSONType::VECTOR2:
		(reinterpret_cast<TVector2F*>(&_data))->~TVector2F();
		break;
	
	case EJSONType::VECTOR3:
		(reinterpret_cast<TVector3F*>(&_data))->~TVector3F();
		break;

	case EJSONType::VECTOR4:
		(reinterpret_cast<TVector4F*>(&_data))->~TVector4F();
		break;

    case EJSONType::STRING:
        (reinterpret_cast<FString*>(&_data))->~FString();
        break;
        
    case EJSONType::ARRAY:
        (reinterpret_cast<JSONArray*>(&_data))->~JSONArray();
        break;
        
    case EJSONType::OBJECT:
        (reinterpret_cast<JSONObject*>(&_data))->~JSONObject();
        break;
        
    default:
        break;
    }
    
    _type = newType;
    
    switch (_type)
    {
	case EJSONType::VECTOR2:
		new(reinterpret_cast<TVector2F*>(&_data)) TVector2F();
		break;

	case EJSONType::VECTOR3:
		new(reinterpret_cast<TVector3F*>(&_data)) TVector3F();
		break;

	case EJSONType::VECTOR4:
		new(reinterpret_cast<TVector3F*>(&_data)) TVector4F();
		break;

    case EJSONType::STRING:
        new(reinterpret_cast<FString*>(&_data)) FString();
        break;
        
    case EJSONType::ARRAY:
        new(reinterpret_cast<JSONArray*>(&_data)) JSONArray();
        break;
        
    case EJSONType::OBJECT:
        new(reinterpret_cast<JSONObject*>(&_data)) JSONObject();
        break;
        
    default:
        break;
    }
}

void FJSONValue::WriteJSONString(FString& dest, const FString& str)
{
    dest += '\"';
    
    for (auto it = str.Begin(); it != str.End(); ++it)
    {
        char c = *it;
        
        if (c >= 0x20 && c != '\"' && c != '\\')
            dest += c;
        else
        {
            dest += '\\';
            
            switch (c)
            {
            case '\"':
            case '\\':
                dest += c;
                break;
                
            case '\b':
                dest += 'b';
                break;
                
            case '\f':
                dest += 'f';
                break;
                
            case '\n':
                dest += 'n';
                break;
                
            case '\r':
                dest += 'r';
                break;
                
            case '\t':
                dest += 't';
                break;
                
            default:
                {
                    char buffer[6];
                    sprintf(buffer, "u%04x", c);
                    dest += buffer;
                }
                break;
            }
        }
    }
    
    dest += '\"';
}

void FJSONValue::WriteIndent(FString& dest, int indent)
{
    size_t oldLength = dest.Length();
    dest.Resize(oldLength + indent);
    for (int i = 0; i < indent; ++i)
        dest[oldLength + i] = ' ';
}

bool FJSONValue::ReadJSONVector2(TVector2F& dest, const char*& pos, const char*& end)
{
	char c;
	if (!NextChar(c, pos, end, true))
		return false;
	if (c == '(')
	{
		if (!NextChar(c, pos, end, true))
			return false;
		// JSON value vector2 x
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._x = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ',' || !NextChar(c, pos, end, true))
			return false;
		// JSON value vector2 y
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._y = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ')')
			return false;
		return true;
	}
	return false;
}

bool FJSONValue::ReadJSONVector3(TVector3F& dest, const char*& pos, const char*& end)
{
	char c;
	if (!NextChar(c, pos, end, true))
		return false;
	if (c == '(')
	{
		if (!NextChar(c, pos, end, true))
			return false;
		// JSON value vector2 x
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._x = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ',' || !NextChar(c, pos, end, true))
			return false;
		// JSON value vector2 y
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._y = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;
		if (!NextChar(c, pos, end, true) || c != ',' || !NextChar(c, pos, end, true))
			return false;
		// JSON value vector4 z
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._z = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ')')
			return false;
		return true;
	}
	return false;
}

bool FJSONValue::ReadJSONVector4(TVector4F& dest, const char*& pos, const char*& end)
{
	char c;
	if (!NextChar(c, pos, end, true))
		return false;
	if (c == '(')
	{
		if (!NextChar(c, pos, end, true))
			return false;
		// JSON value vector4 x
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._x = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ',' || !NextChar(c, pos, end, true))
			return false;
		// JSON value vector4 y
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._y = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ',' || !NextChar(c, pos, end, true))
			return false;
		// JSON value vector4 z
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._z = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ',' || !NextChar(c, pos, end, true))
			return false;
		// JSON value vector4 w
		if (IsDigit(c) || c == '-')
		{
			--pos;
			dest._w = strtod(pos, const_cast<char**>(&pos));
		}
		else
			return false;

		if (!NextChar(c, pos, end, true) || c != ')')
			return false;
		return true;
	}
	return false;
}

bool FJSONValue::ReadJSONString(FString& dest, const char*& pos, const char*& end, bool inQuote)
{
    char c;
    
    if (!inQuote)
    {
        if (!NextChar(c, pos, end, true) || c != '\"')
            return false;
    }
    
    dest.Clear();
    for (;;)
    {
        if (!NextChar(c, pos, end, false))
            return false;
        if (c == '\"')
            break;
        else if (c != '\\')
            dest += c;
        else
        {
            if (!NextChar(c, pos, end, false))
                return false;
            switch (c)
            {
            case '\\':
                dest += '\\';
                break;
                
            case '\"':
                dest += '\"';
                break;
                
            case 'b':
                dest += '\b';
                break;
                
            case 'f':
                dest += '\f';
                break;
                
            case 'n':
                dest += '\n';
                break;
            
            case 'r':
                dest += '\r';
                break;
                
            case 't':
                dest += '\t';
                break;
                
            case 'u':
                {
                    /// \todo Doesn't _handle unicode surrogate pairs
                    unsigned code;
                    sscanf(pos, "%x", &code);
                    pos += 4;
                    dest.AppendUTF8(code);
                }
                break;
            }
        }
    }
    
    return true;
}

bool FJSONValue::MatchString(const char* str, const char*& pos, const char*& end)
{
    while (*str)
    {
        if (pos >= end || *pos != *str)
            return false;
        else
        {
            ++pos;
            ++str;
        }
    }
    
    return true;
}

bool FJSONValue::MatchChar(char c, const char*& pos, const char*& end)
{
    char next;

    while (NextChar(next, pos, end, false))
    {
        if (next == c)
            return true;
    }
    return false;
}

}
