#pragma once
#include <rttr/string_view.h>
#include <rttr/registration.h>
#include <rttr/type>
#include <rttr/registration_friend.h>

#include "Container/String.h"

namespace Auto3D
{

using FRegistration = rttr::registration;
using FType = rttr::type;
using FVariant = rttr::variant;
using FConstructor = rttr::constructor;
using FProperty = rttr::property;
using FMethod = rttr::method;

#define SERIALIZABLE "Serializable"

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