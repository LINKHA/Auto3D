#include "../Base/String.h"
#include "../Base/Swap.h"
#include "Rect.h"

#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const Rect Rect::FULL(-1.0f, -1.0f, 1.0f, 1.0f);
const Rect Rect::POSITIVE(0.0f, 0.0f, 1.0f, 1.0f);
const Rect Rect::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

void Rect::Clip(const Rect& rect)
{
    if (rect._min._x > _min._x)
        _min._x = rect._min._x;
    if (rect._max._x < _max._x)
        _max._x = rect._max._x;
    if (rect._min._y > _min._y)
        _min._y = rect._min._y;
    if (rect._max._y < _max._y)
        _max._y = rect._max._y;
    
    if (_min._x > _max._x)
        Swap(_min._x, _max._x);
    if (_min._y > _max._y)
        Swap(_min._y, _max._y);
}

bool Rect::FromString(const String& str)
{
    return FromString(str.CString());
}

bool Rect::FromString(const char* str)
{
    size_t elements = String::CountElements(str, ' ');
    if (elements < 4)
        return false;
    
    char* ptr = (char*)str;
    _min._x = (float)strtod(ptr, &ptr);
    _min._y = (float)strtod(ptr, &ptr);
    _max._x = (float)strtod(ptr, &ptr);
    _max._y = (float)strtod(ptr, &ptr);
    
    return true;
}

String Rect::ToString() const
{
    return _min.ToString() + " " + _max.ToString();
}

}
