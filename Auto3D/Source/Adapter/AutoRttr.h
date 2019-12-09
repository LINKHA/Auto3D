#pragma once
#include "rttr/registration.h"
#include "rttr/string_view.h"
#include "Container/String.h"

namespace Auto3D
{

using Registration = rttr::registration;
using Type = rttr::type;
using Variant = rttr::variant;
using Constructor = rttr::constructor;
using Property = rttr::property;
using Method = rttr::method;

inline rttr::string_view ToRtStr(const FString& str)
{
	return rttr::string_view(str.CString());
}

inline rttr::string_view ToRtStr(const char* str)
{
	return rttr::string_view(str);
}

inline FString RtToStr(rttr::string_view str)
{
	return FString(str.to_string().c_str());
}

}
#define AUTO_RTTR_REGISTRATION RTTR_REGISTRATION