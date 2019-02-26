#include "../Base/Vector.h"
#include "../Base/HashMap.h"
#include "JSONValue.h"
#include "Stream.h"

#include <cstdio>
#include <cstdlib>

namespace Auto3D
{

const JSONValue JSONValue::EMPTY;
const JSONArray JSONValue::emptyJSONArray;
const JSONObject JSONValue::emptyJSONObject;

JSONValue::JSONValue() :
    _type(JSON_NULL)
{
}

JSONValue::JSONValue(const JSONValue& value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(bool value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(int value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(unsigned value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(float value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(double value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(const String& value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(const char* value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(const JSONArray& value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::JSONValue(const JSONObject& value) :
    _type(JSON_NULL)
{
    *this = value;
}

JSONValue::~JSONValue()
{
    SetType(JSON_NULL);
}

JSONValue& JSONValue::operator = (const JSONValue& rhs)
{
    SetType(rhs._type);
    
    switch (_type)
    {
    case JSON_BOOL:
        _data.boolValue = rhs._data.boolValue;
        break;
        
    case JSON_NUMBER:
        _data.numberValue = rhs._data.numberValue;
        break;
        
    case JSON_STRING:
        *(reinterpret_cast<String*>(&_data)) = *(reinterpret_cast<const String*>(&rhs._data));
        break;
        
    case JSON_ARRAY:
        *(reinterpret_cast<JSONArray*>(&_data)) = *(reinterpret_cast<const JSONArray*>(&rhs._data));
        break;
        
    case JSON_OBJECT:
        *(reinterpret_cast<JSONObject*>(&_data)) = *(reinterpret_cast<const JSONObject*>(&rhs._data));
        break;
        
    default:
        break;
    }
    
    return *this;
}

JSONValue& JSONValue::operator = (bool rhs)
{
    SetType(JSON_BOOL);
    _data.boolValue = rhs;
    return *this;
}

JSONValue& JSONValue::operator = (int rhs)
{
    SetType(JSON_NUMBER);
    _data.numberValue = (double)rhs;
    return *this;
}

JSONValue& JSONValue::operator = (unsigned rhs)
{
    SetType(JSON_NUMBER);
    _data.numberValue = (double)rhs;
    return *this;
}

JSONValue& JSONValue::operator = (float rhs)
{
    SetType(JSON_NUMBER);
    _data.numberValue = (double)rhs;
    return *this;
}

JSONValue& JSONValue::operator = (double rhs)
{
    SetType(JSON_NUMBER);
    _data.numberValue = rhs;
    return *this;
}

JSONValue& JSONValue::operator = (const String& value)
{
    SetType(JSON_STRING);
    *(reinterpret_cast<String*>(&_data)) = value;
    return *this;
}

JSONValue& JSONValue::operator = (const char* value)
{
    SetType(JSON_STRING);
    *(reinterpret_cast<String*>(&_data)) = value;
    return *this;
}

JSONValue& JSONValue::operator = (const JSONArray& value)
{
    SetType(JSON_ARRAY);
    *(reinterpret_cast<JSONArray*>(&_data)) = value;
    return *this;
}

JSONValue& JSONValue::operator = (const JSONObject& value)
{
    SetType(JSON_OBJECT);
    *(reinterpret_cast<JSONObject*>(&_data)) = value;
    return *this;
}

JSONValue& JSONValue::operator [] (size_t index)
{
    if (_type != JSON_ARRAY)
        SetType(JSON_ARRAY);
    
    return (*(reinterpret_cast<JSONArray*>(&_data)))[index];
}

const JSONValue& JSONValue::operator [] (size_t index) const
{
    if (_type == JSON_ARRAY)
        return (*(reinterpret_cast<const JSONArray*>(&_data)))[index];
    else
        return EMPTY;
}

JSONValue& JSONValue::operator [] (const String& key)
{
    if (_type != JSON_OBJECT)
        SetType(JSON_OBJECT);
    
    return (*(reinterpret_cast<JSONObject*>(&_data)))[key];
}

const JSONValue& JSONValue::operator [] (const String& key) const
{
    if (_type == JSON_OBJECT)
    {
        const JSONObject& object = *(reinterpret_cast<const JSONObject*>(&_data));
        auto it = object.Find(key);
        return it != object.End() ? it->second : EMPTY;
    }
    else
        return EMPTY;
}

bool JSONValue::operator == (const JSONValue& rhs) const
{
    if (_type != rhs._type)
        return false;
    
    switch (_type)
    {
    case JSON_BOOL:
        return _data.boolValue == rhs._data.boolValue;
        
    case JSON_NUMBER:
        return _data.numberValue == rhs._data.numberValue;
        
    case JSON_STRING:
        return *(reinterpret_cast<const String*>(&_data)) == *(reinterpret_cast<const String*>(&rhs._data));
        
    case JSON_ARRAY:
        return *(reinterpret_cast<const JSONArray*>(&_data)) == *(reinterpret_cast<const JSONArray*>(&rhs._data));
        
    case JSON_OBJECT:
        return *(reinterpret_cast<const JSONObject*>(&_data)) == *(reinterpret_cast<const JSONObject*>(&rhs._data));
        
    default:
        return true;
    }
}

bool JSONValue::FromString(const String& str)
{
    const char* pos = str.CString();
    const char* end = pos + str.Length();
    return Parse(pos, end);
}

bool JSONValue::FromString(const char* str)
{
    const char* pos = str;
    const char* end = pos + String::CStringLength(str);
    return Parse(pos, end);
}

void JSONValue::FromBinary(Stream& source)
{
    JSONType newType = (JSONType)source.Read<unsigned char>();

    switch (newType)
    {
    case JSON_NULL:
        Clear();
        break;

    case JSON_BOOL:
        *this = source.Read<bool>();
        break;

    case JSON_NUMBER:
        *this = source.Read<double>();
        break;

    case JSON_STRING:
        *this = source.Read<String>();
        break;

    case JSON_ARRAY:
        {
            SetEmptyArray();
            size_t num = source.ReadVLE();
            for (size_t i = 0; i < num && !source.IsEof(); ++i)
                Push(source.Read<JSONValue>());
        }
        break;

    case JSON_OBJECT:
        {
            SetEmptyObject();
            size_t num = source.ReadVLE();
            for (size_t i = 0; i < num && !source.IsEof(); ++i)
            {
                String key = source.Read<String>();
                (*this)[key] = source.Read<JSONValue>();
            }
        }
        break;

    default:
        break;
    }
}

void JSONValue::ToString(String& dest, int spacing, int indent) const
{
    switch (_type)
    {
    case JSON_BOOL:
        dest += _data.boolValue;
        return;
        
    case JSON_NUMBER:
        dest += _data.numberValue;
        return;
        
    case JSON_STRING:
        WriteJSONString(dest, *(reinterpret_cast<const String*>(&_data)));
        return;
        
    case JSON_ARRAY:
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
        
    case JSON_OBJECT:
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
                    WriteJSONString(dest, it->first);
                    dest += ": ";
                    it->second.ToString(dest, spacing, indent);
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

String JSONValue::ToString(int spacing) const
{
    String ret;
    ToString(ret, spacing);
    return ret;
}

void JSONValue::ToBinary(Stream& dest) const
{
    dest.Write((unsigned char)_type);

    switch (_type)
    {
    case JSON_BOOL:
        dest.Write(_data.boolValue);
        break;

    case JSON_NUMBER:
        dest.Write(_data.numberValue);
        break;

    case JSON_STRING:
        dest.Write(GetString());
        break;

    case JSON_ARRAY:
        {
            const JSONArray& array = GetArray();
            dest.WriteVLE(array.Size());
            for (auto it = array.Begin(); it != array.End(); ++it)
                it->ToBinary(dest);
        }
        break;

    case JSON_OBJECT:
        {
            const JSONObject& object = GetObject();
            dest.WriteVLE(object.Size());
            for (auto it = object.Begin(); it != object.End(); ++it)
            {
                dest.Write(it->first);
                it->second.ToBinary(dest);
            }
        }
        break;

    default:
        break;
    }
}

void JSONValue::Push(const JSONValue& value)
{
    SetType(JSON_ARRAY);
    (*(reinterpret_cast<JSONArray*>(&_data))).Push(value);
}

void JSONValue::Insert(size_t index, const JSONValue& value)
{
    SetType(JSON_ARRAY);
    (*(reinterpret_cast<JSONArray*>(&_data))).Insert(index, value);
}

void JSONValue::Pop()
{
    if (_type == JSON_ARRAY)
        (*(reinterpret_cast<JSONArray*>(&_data))).Pop();
}

void JSONValue::Erase(size_t pos, size_t length)
{
    if (_type == JSON_ARRAY)
        (*(reinterpret_cast<JSONArray*>(&_data))).Erase(pos, length);
}

void JSONValue::Resize(size_t newSize)
{
    SetType(JSON_ARRAY);
    (*(reinterpret_cast<JSONArray*>(&_data))).Resize(newSize);
}

void JSONValue::Insert(const Pair<String, JSONValue>& pair)
{
    SetType(JSON_OBJECT);
    (*(reinterpret_cast<JSONObject*>(&_data))).Insert(pair);
}

void JSONValue::Erase(const String& key)
{
    if (_type == JSON_OBJECT)
        (*(reinterpret_cast<JSONObject*>(&_data))).Erase(key);
}

void JSONValue::Clear()
{
    if (_type == JSON_ARRAY)
        (*(reinterpret_cast<JSONArray*>(&_data))).Clear();
    else if (_type == JSON_OBJECT)
        (*(reinterpret_cast<JSONObject*>(&_data))).Clear();
}

void JSONValue::SetEmptyArray()
{
    SetType(JSON_ARRAY);
    Clear();
}

void JSONValue::SetEmptyObject()
{
    SetType(JSON_OBJECT);
    Clear();
}

void JSONValue::SetNull()
{
    SetType(JSON_NULL);
}

size_t JSONValue::Size() const
{
    if (_type == JSON_ARRAY)
        return (*(reinterpret_cast<const JSONArray*>(&_data))).Size();
    else if (_type == JSON_OBJECT)
        return (*(reinterpret_cast<const JSONObject*>(&_data))).Size();
    else
        return 0;
}

bool JSONValue::IsEmpty() const
{
    if (_type == JSON_ARRAY)
        return (*(reinterpret_cast<const JSONArray*>(&_data))).IsEmpty();
    else if (_type == JSON_OBJECT)
        return (*(reinterpret_cast<const JSONObject*>(&_data))).IsEmpty();
    else
        return false;
}

bool JSONValue::Contains(const String& key) const
{
    if (_type == JSON_OBJECT)
        return (*(reinterpret_cast<const JSONObject*>(&_data))).Contains(key);
    else
        return false;
}

bool JSONValue::Parse(const char*& pos, const char*& end)
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
    else if (c == 'n')
    {
        SetNull();
        return MatchString("ull", pos, end);
    }
    else if (c == 'f')
    {
        *this = false;
        return MatchString("alse", pos, end);
    }
    else if (c == 't')
    {
        *this = true;
        return MatchString("rue", pos, end);
    }
    else if (IsDigit(c) || c == '-')
    {
        --pos;
        *this = strtod(pos, const_cast<char**>(&pos));
        return true;
    }
    else if (c == '\"')
    {
        SetType(JSON_STRING);
        return ReadJSONString(*(reinterpret_cast<String*>(&_data)), pos, end, true);
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
            JSONValue arrayValue;
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
            String key;
            if (!ReadJSONString(key, pos, end, false))
                return false;
            if (!NextChar(c, pos, end, true))
                return false;
            if (c != ':')
                return false;
            
            JSONValue objectValue;
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

void JSONValue::SetType(JSONType newType)
{
    if (_type == newType)
        return;
    
    switch (_type)
    {
    case JSON_STRING:
        (reinterpret_cast<String*>(&_data))->~String();
        break;
        
    case JSON_ARRAY:
        (reinterpret_cast<JSONArray*>(&_data))->~JSONArray();
        break;
        
    case JSON_OBJECT:
        (reinterpret_cast<JSONObject*>(&_data))->~JSONObject();
        break;
        
    default:
        break;
    }
    
    _type = newType;
    
    switch (_type)
    {
    case JSON_STRING:
        new(reinterpret_cast<String*>(&_data)) String();
        break;
        
    case JSON_ARRAY:
        new(reinterpret_cast<JSONArray*>(&_data)) JSONArray();
        break;
        
    case JSON_OBJECT:
        new(reinterpret_cast<JSONObject*>(&_data)) JSONObject();
        break;
        
    default:
        break;
    }
}

void JSONValue::WriteJSONString(String& dest, const String& str)
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

void JSONValue::WriteIndent(String& dest, int indent)
{
    size_t oldLength = dest.Length();
    dest.Resize(oldLength + indent);
    for (int i = 0; i < indent; ++i)
        dest[oldLength + i] = ' ';
}

bool JSONValue::ReadJSONString(String& dest, const char*& pos, const char*& end, bool inQuote)
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

bool JSONValue::MatchString(const char* str, const char*& pos, const char*& end)
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

bool JSONValue::MatchChar(char c, const char*& pos, const char*& end)
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
